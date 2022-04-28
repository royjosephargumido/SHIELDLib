#include "SHIELDLib.h"

File file;          // File System
RTC_DS3231 rtc;     // RTC Module
WiFiUDP ntpUDP;     // Network Time Protocol (used to sync RTC with internet time)
SoftwareSerial ble(D3, D4);     // BLE Module
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);    // Required for the syncing of local time with the internet time
Adafruit_SSD1306 deviceOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);        // OLED module


// Cryptography
uint32_t prof_start_time        = 0;
uint32_t profile_interval       = 15;    //Change this to reflect the interval for the next Profile issuance (in minutes)
const uint32_t PROFILE_PERIOD   = profile_interval * 60;

uint32_t circ_start_time        = 0;
const uint32_t CIRCADIAN_PERIOD = 86400;   // Number of seconds in a day

uint32_t currentSN = 0;     // Current SequenceNumber
bool ftb = true;            // First-time Boot (use to check if the device boots for the first time)

String smart_tag = "";

ICACHE_FLASH_ATTR SHIELDLib::SHIELDLib() {
	lastYield = 0;
}

/* CORE FUNCTIONALITIES */

void SHIELDLib::startDevice() {
    initOLED();     // Initializes the OLED display
    initSDCard();   // Initializes the SD Card module
    beginClock();   // Begins the clock
    openBLE();      // Opens the BLE module

    syncClock();    // Conncets to a saved WiFi and syncs local time with internet time

    //Sets the beggining SequenceNumber for the issuance of Circadian and Profile
    uint32_t currSN = getSequenceNumber();
    circ_start_time = currSN + CIRCADIAN_PERIOD;
    prof_start_time = currSN + PROFILE_PERIOD;
}

void SHIELDLib::displayDateTime() {
    if(beginClock()) {
        DateTime now = rtc.now();

        // Adding the '0' Padding to minute if minute is lesser than 10
        String Min = (now.minute() < 10) ? "0" + (String)now.minute() : (String)now.minute();
        String Sec = (now.second() < 10) ? "0" + (String)now.second() : (String)now.second();

        String Date =  (String)now.month() + '/' + (String)now.day() + '/' + now.year();
        
        String Time = "";
        if(now.hour() > 12) {   //Check if PM
            Time = (String)(now.hour() % 12) + ':' + Min  + ":" + Sec;
        } else {    // Time is AM
            Time = (now.hour() == 0) ? "12" : (String)now.hour();            
            Time += ':' + Min + ":" + Sec;
        }
        
        char _time[12];
        char _date[11];
        Time.toCharArray(_time, 11);
        Date.toCharArray(_date, 10);

        displayMessage(_time, _date);
    }
}

void SHIELDLib::protocolbegin() {
    currentSN = getSequenceNumber();
    char* _dt = (char*)malloc(50);
    
    //CIRCADIAN
    if(currentSN == circ_start_time || ftb) {        
        String circadian = trng(CIRCADIAN, MAX_BLOCKS);      //Generates the Circadian
        save(CIRCADIAN_DATA, circadian);

        sprintf(_dt, "%02d/%02d/%02d %02d:%02d:%02d", day(currentSN), month(currentSN), year(currentSN), hour(currentSN), minute(currentSN), second(currentSN));
        Serial.println(_dt);
        Serial.println("=========================================");
        Serial.println("CIRCADIAN:\t" + circadian);
        Serial.println("=========================================");
        Serial.println();
        Serial.println();

        if(!ftb) { circ_start_time += CIRCADIAN_PERIOD; }
    }    
    
    //SmartTag
    if(currentSN == prof_start_time || ftb) {
        // PUK
        unsigned char salt_puk[MAX_BLOCKS] = {};
        trng(salt_puk, 16);                                         //Generate the salts for PUK using TRNG
        String puk = perfHKDF(CIRCADIAN, salt_puk, INFO_PUK, 16);   //Generate the PUK using HKDF
        byte PUK[MAX_BLOCKS];                                       // Profile Unlocking Key
        stringtobyte(PUK, puk);                                     //Store the generated PUK

        // TUK
        unsigned char salt_tuk[MAX_BLOCKS] = {};
        trng(salt_tuk, 16);                                         //Generate the salts for TUK using TRNG
        String tuk = perfHKDF(CIRCADIAN, salt_tuk, INFO_TUK, 16);   //Generate the TUK using HKDF
        byte TUK[MAX_BLOCKS];                                       // Transcript Unlocking Key
        stringtobyte(TUK, tuk);                                     //Store the generated TUK

        //PID
        String pid = perfHKDF(CIRCADIAN, PUK, INFO_PID, 16);        //Generate the PID using HKDF
        byte PID[MAX_BLOCKS];                                       // Profile Identifier
        stringtobyte(PID, pid);                                     //Store the generated PID

        //Encrpyt the PUK using the Circadian and the TUK
        smart_tag = encrypt(CIRCADIAN, PUK, TUK);               // Encrypts PUK using CIRCADIAN and TUK in AES128-CTR

        //Combine PUK, PID, and TUK to become as the Profile as a JSON document
        String profile = "";
        StaticJsonDocument<192> doc;

        doc["SN"] = SNtoString(currentSN);
        doc["PUK"] = puk;
        doc["PID"] = pid;
        doc["TUK"] = tuk;

        serializeJson(doc, profile);

        sprintf(_dt, "%02d/%02d/%02d %02d:%02d:%02d", day(currentSN), month(currentSN), year(currentSN), hour(currentSN), minute(currentSN), second(currentSN));
        Serial.println(_dt);
        Serial.println("=========================================");
        Serial.print("Smart Tag:\n");
        Serial.println(smart_tag);
        Serial.println("=========================================");

        /*
        Serial.println("PUK:\t" + puk);
        Serial.println("TUK:\t" + tuk);
        Serial.println("PID:\t" + pid);
        Serial.println("=========================================");
        Serial.println("Profile:\n" + cipher);
        */
        Serial.println();
        Serial.println();
          
        save(PROFILE_DATA, profile);        //Save Profile (PUK, PID, and TUK)
        save(SMARTTAG_DATA, smart_tag);     //Save Smart Tag

        if(!ftb) { prof_start_time += PROFILE_PERIOD; }
    }

    if(ftb) { ftb = false; }
}

void SHIELDLib::listen() {
    String message = "";
    message.trim();

    if(ble.available() > 0) {
        message = ble.readString();
    }

    if(message.length() == 11) {
        Serial.println("Connected.");
    }

    if(message.length() >= 15) {
        message.trim();
        Serial.println("Data received.");
        Serial.println(message.length());
        Serial.println(message);
        //save(TRANSCRIPT_DATA, message);
        ble.println(smart_tag);
        //Serial.flush();
        //ble.flush();
    }
}

/* HARDWARE COMPONENTS */

void SHIELDLib::initOLED() {
    if(!deviceOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("OLED Display failed to start."));
        for(;;);
    }
    displayMessage("Starting", "SHIELD...");
}

void SHIELDLib::initSDCard() {
    pinMode(pin_csSD, OUTPUT);

    while(!SD.begin(pin_csSD)) {
        Serial.println(F("SD Card missing"));
        displayError(SD_MISSING);
        delay(50);
    }
}

bool SHIELDLib::beginClock() {
    while(!rtc.begin()) {
        Serial.println(F("RTC missing"));
        displayError(CLOCK_MISSING);        
        delay(50);
        return false;
    }
    return true;
}

void SHIELDLib::openBLE() {
    ble.begin(9600);
}

/* UTILITIES */

const char *ssid     = "ODIMUGRA";
const char *password = "odimugra023026";

void SHIELDLib::syncClock() {
    DateTime now = rtc.now();
    uint32_t _unixtime = now.unixtime();

    if(_unixtime < 1650652888) {
        displayMessage("Clock", "Syncing...");

        WiFi.begin(ssid, password);

        Serial.println("Connecting...");
        while ( WiFi.status() != WL_CONNECTED ) {
            delay ( 500 );
        }
        Serial.println("Connected.");

        timeClient.begin();

        int timer = 0;
        unsigned long _t = 0;

        do{
            timeClient.update();
            _t = timeClient.getEpochTime();
            timer += 1;
        }while (timer < 2);

        rtc.adjust(DateTime(year(_t), month(_t), day(_t), hour(_t), minute(_t), second(_t)));
        WiFi.disconnect();
    }
}

void SHIELDLib::connecttoWIFI(char* wifi_ssid, char* wifi_password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED){}
}

void SHIELDLib::displayError(ErrorCodes err) {
    switch (err)
    {
    case CLOCK_MISSING:
        displayMessage("ERROR!", "No RTC Mod");
        break;

    case SD_MISSING:
        displayMessage("ERROR!", "No SD Card");
        break;
    
    default:
        break;
    }
}

void SHIELDLib::displayMessage(char* line1, char* line2) {
    deviceOLED.clearDisplay();          // Clear the buffer.
    deviceOLED.setTextSize(2);
    deviceOLED.setTextColor(WHITE);
    deviceOLED.setCursor(0,0);
    deviceOLED.println(line1);
    deviceOLED.println(line2);
    deviceOLED.display();
}

/* FILE SYSTEM */

void SHIELDLib::save(FileToSave _destinationFile, String _rawdata) {
    String __destination = getFilename(_destinationFile, SNtoString(getSequenceNumber()));

    do {
        file = SD.open(__destination, FILE_WRITE);
        if(file) {
            //Serial.println("Unable to create the file!");
            delay(500);
        }
    }while(!file);

    file.print(_rawdata);
    file.close();
}

String SHIELDLib::getFilename(FileToSave _SHIELDFile, String SequenceNumber) {
    String _dest = "";
    switch(_SHIELDFile){
        case 0: //Audit Folder
            _dest = dir_audit + _slash + "audit_" + SequenceNumber + file_extension;
            break;
        
        case 1: //Circadian Folder
            _dest = dir_circadian + _slash + "circadian_" + SequenceNumber + file_extension;
            break;

        case 2: //Cirrus Folder
            _dest = dir_cirrus + _slash + "cirrus" + file_extension;
            break;

        case 3: //Transcript Folder
            _dest = dir_memories + _slash + "transcript_" + SequenceNumber + file_extension;
            break;

        case 4: //Dump Folder
            _dest = dir_dumps + _slash + "dumps_" + SequenceNumber + file_extension;
            break;

        case 5: //Config Folder
            _dest = dir_core + _slash + "beaconconfig" + file_extension;
            break;

        case 6: //Profile Folder
            _dest = dir_profile + _slash + "profile_" + SequenceNumber + file_extension;
            break;
    }

    return _dest;
}

/* SHIELD'S CRYPTOGRAPHY FUNCTIONS */

HKDF<SHA256> hkdf_hmac_object;      // Creates the HKDF Object based on HMAC-SHA256
CTR<AES128> ctraes128;              // Creates the AES Object in AES128-CTR Mode

uint32_t SHIELDLib::getSequenceNumber() {
    DateTime now = rtc.now();
    return now.unixtime();
}

ICACHE_FLASH_ATTR String  SHIELDLib::trng(uint8_t* location, int outputLength) {
    //Fills the array with random bytes
    char* pos = (char*)location;
    int size = outputLength;
    for (;size--;) *pos++ = randomByte();

    //Converts the byte array to its Hexadecimal/String representation
    String string = "";
    for (int index = 0; index < outputLength; index++) {
        int topDigit = location[index] >> 4;
        int bottomDigit = location[index] & 0x0f;
        string += "0123456789abcdef"[topDigit];     // High Hex digit
        string += "0123456789abcdef"[bottomDigit];  // Low Hex digit
    }

    return string;
}

String SHIELDLib::perfHKDF(const unsigned char *key, const unsigned char *salt, const unsigned char *info, size_t outputlength) {
    uint8_t outs[MAX_BLOCKS * 2];

    HKDFCommon *_hkdf = &hkdf_hmac_object;

    size_t size = outputlength;
    size_t posn, len;

    _hkdf->setKey(key, 16, salt, 16);

    for (posn = 0; posn < size; posn += outputlength) {
        len = size - posn;
        if (len > outputlength)
            len = outputlength;
        _hkdf->extract(outs + posn, len, info, sizeof(info));
    }

    return bytetostring(outs);
}

String SHIELDLib::encrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce) {
    byte output[MAX_BLOCKS];
    size_t posn, len;

    Cipher *cipher = &ctraes128;

    cipher->clear();
    cipher->setKey(key, MAX_BLOCKS);
    cipher->setIV(nonce, MAX_BLOCKS);

    //Fill the array with the hexadecimal 0xBA
    memset(output, 0xBA, sizeof(output));

    int inc = MAX_BLOCKS;
    for (posn = 0; posn < MAX_BLOCKS; posn += inc) {
        len = MAX_BLOCKS - posn;
        if (len > inc)
            len = inc;
        cipher->encrypt(output + posn, data + posn, len);
    }
    return bytetostring(output);
}

String SHIELDLib::decrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce) {
    byte output[MAX_BLOCKS];
    size_t posn, len;

    Cipher *cipher = &ctraes128;

    cipher->setKey(key, MAX_BLOCKS);
    cipher->setIV(nonce, MAX_BLOCKS);

    int inc = MAX_BLOCKS;
    for (posn = 0; posn < MAX_BLOCKS; posn += inc) {
        len = MAX_BLOCKS - posn;
        if (len > inc)
            len = inc;
        cipher->decrypt(output + posn, data + posn, len);
    }

    return bytetostring(output);
}

/* CRYPTOGRAPHY UTILITIES */

String SHIELDLib::SNtoString(uint32_t epoch) {
    char* _dt = (char*)malloc(50);
    ltoa(epoch, _dt, 10);
    return String(_dt);
}

String SHIELDLib::bytetostring(byte array[]) {
    char buffer[MAX_BLOCKS * 2];

    for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xa ? '0' + nib1  : 'a' + nib1  - 0xa;
        buffer[i*2+1] = nib2  < 0xa ? '0' + nib2  : 'a' + nib2  - 0xa;
    }
    buffer[sizeof(buffer)] = '\0';

    return String(buffer);
}

void SHIELDLib::stringtobyte(uint8_t* location, String rawdata) {
    char buffer[MAX_BLOCKS * 2 + 1];
    rawdata.toCharArray(buffer, MAX_BLOCKS * 2 + 1);

    int pos = 0;
    for(int index = 0; index < MAX_BLOCKS * 2; pos++, ++index) {
        auto getNum = [](char c){ return c > '9' ? c - 'a' + 10 : c - '0'; };
        location[pos] = (getNum((char)buffer[index]) << 4) + getNum((char)buffer[++index]);
    }
}

/* TRNG UTILITIES */

ICACHE_FLASH_ATTR int SHIELDLib::randomBit(void) {
    // Needed to keep wifi stack running smoothly and to avoid wdt reset
    if (lastYield == 0 || millis() - lastYield >= 50) {
    yield();
    lastYield = millis();
    }
    uint8_t bit = (int)RANDOM_REG32;  //This uses the onboard Random Number Generator (esp8266_peri.h)

    return bit & 1;
}

ICACHE_FLASH_ATTR int SHIELDLib::whiten(void) {
    /* 
    * Software whitening bits using Von Neuman Algorithm which fixes simple bias and reduce correlation
    * 
    * VNA considers two bits at a time (non-overlapping), taking one of three actions:
    *  - when two successive bits are equal, they are discarded;
    *  - a sequence of 1,0 becomes a 1;
    *  - and a sequence of 0,1 becomes a zero.
    * It thus represents a falling edge with a 1, and a rising edge with a 0.
    * 
    * See:
    * https://dornsifecms.usc.edu/assets/sites/520/docs/VonNeumann-ams12p36-38.pdf
    * https://en.wikipedia.org/wiki/Hardware_random_number_generator#Software_whitening
    */
    for(;;) { /* will loop forever until a value is returned, does not execute the last return*/
        int a = randomBit() | randomBit() << 1;
        if (a==1) return 0; // 1 to 0 transition: log a zero bit
        if (a==2) return 1; // 0 to 1 transition: log a one bit
        // For other cases, try again.
    }

    return 0;
}

ICACHE_FLASH_ATTR char SHIELDLib::randomByte(void) {
    char result = 0;
    for (uint8_t i = 8; i--;)
        result += result + whiten();
    return result;
}

SHIELDLib shield;