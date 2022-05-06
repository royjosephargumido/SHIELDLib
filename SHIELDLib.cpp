#include "SHIELDLib.h"

File file;                      // File System
RTC_DS3231 rtc;                 // RTC Module
WiFiUDP ntpUDP;                 // Network Time Protocol (used to sync RTC with internet time)
PCF8574 pcf8574(0x20);          // IO Expander Module
SoftwareSerial ble(D3, D4);     // BLE Module
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);    // Required for the syncing of local time with the internet time
Adafruit_SSD1306 deviceOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);        // OLED module

//========================= CACHE =========================
uint32_t profile_interval;
uint32_t EOS;
String HealthStatus = "";
//=========================================================

uint32_t prof_start_time        = 0;
const uint32_t PROFILE_PERIOD   = profile_interval * 60;

uint32_t circ_start_time        = 0;
const uint32_t CIRCADIAN_PERIOD = 86400;   // Number of seconds in a day

uint32_t currentSN = 0;     // Current SequenceNumber
bool ftb = true;            // First-time Boot (use to check if the device boots for the first time)
String smart_tag = "";
StaticJsonDocument<200> doc;
StaticJsonDocument<200> json_config;

/**
 * @brief SHIELD's Constructor
 * 
 * @return ICACHE_FLASH_ATTR 
 */
ICACHE_FLASH_ATTR SHIELDLib::SHIELDLib() {
	lastYield = 0;
}

/* CORE FUNCTIONALITIES */

void SHIELDLib::startDevice() {
    initOLED();         // Initializes the OLED display
    initSDCard();       // Initializes the SD Card module
    beginClock();       // Begins the clock
    openBLE();          // Opens the BLE module
    initIOExpander();   //Starts the IO Expander Module

    syncClock();        // Conncets to a saved WiFi and syncs local time with internet time

    Settings();         //Opens or writes default configuration
}

void SHIELDLib::protocolbegin() {
    currentSN = getSequenceNumber();
    char* _dt = (char*)malloc(50);
    
    if(currentSN == circ_start_time || ftb) {        
        String circadian = trng(CIRCADIAN, MAX_BLOCKS);      //Generates the Circadian
        save(CIRCADIAN_DATA, circadian);

        sprintf(_dt, "%02d/%02d/%02d %02d:%02d:%02d", day(currentSN), month(currentSN), year(currentSN), hour(currentSN), minute(currentSN), second(currentSN));
        Serial.println("===============================================");
        Serial.print("Generated:\t");
        Serial.println(_dt);
        Serial.println("CIRCADIAN:\t" + circadian);
        Serial.println("===============================================");
        Serial.println();
        Serial.println();

        if(!ftb) { circ_start_time += CIRCADIAN_PERIOD; }
    }
    
    if(currentSN == prof_start_time || ftb) {
        // PUK
        unsigned char salt_puk[MAX_BLOCKS] = {};
        trng(salt_puk, 16);                                         //Generate the salts for PUK using TRNG
        String puk = perfHKDF(CIRCADIAN, salt_puk, INFO_PUK, 16);   //Generate the PUK using HKDF
        byte PUK[MAX_BLOCKS];                                       //Profile Unlocking Key
        stringtobyte(PUK, puk);                                     //Store the generated PUK

        // TUK
        unsigned char salt_tuk[MAX_BLOCKS] = {};
        trng(salt_tuk, 16);                                         //Generate the salts for TUK using TRNG
        String tuk = perfHKDF(CIRCADIAN, salt_tuk, INFO_TUK, 16);   //Generate the TUK using HKDF
        byte TUK[MAX_BLOCKS];                                       //Transcript Unlocking Key
        stringtobyte(TUK, tuk);                                     //Store the generated TUK

        //PID
        String pid = perfHKDF(CIRCADIAN, PUK, INFO_PID, 16);        //Generate the PID using HKDF
        byte PID[MAX_BLOCKS];                                       //Profile Identifier
        stringtobyte(PID, pid);                                     //Store the generated PID

        //Encrpyt the PUK using the Circadian and the TUK
        smart_tag = encrypt(CIRCADIAN, PUK, TUK);                   //Encrypts PUK using CIRCADIAN and TUK in AES128-CTR
        //uint32_t CV = getCIRRUSVersion();
        //cipher = ulongtoString(currentSN) + '-' + cipher;

        String profile = "";

        doc["SN"] = ulongtoString(currentSN);
        doc["PUK"] = puk;
        doc["PID"] = pid;
        doc["TUK"] = tuk;

        serializeJson(doc, profile);

        sprintf(_dt, "%02d/%02d/%02d %02d:%02d:%02d", day(currentSN), month(currentSN), year(currentSN), hour(currentSN), minute(currentSN), second(currentSN));
        Serial.println("===============================================");
        Serial.print("Generated:\t");
        Serial.println(_dt);
        Serial.print("SmartTag:\t");
        Serial.println(smart_tag);
        Serial.println("===============================================");

        /*
        Serial.println("PUK:\t" + puk);
        Serial.println("TUK:\t" + tuk);
        Serial.println("PID:\t" + pid);
        Serial.println("=========================================");
        Serial.println("Profile:\n" + cipher);
        */
        Serial.println();
          
        save(SMARTTAG_DATA, smart_tag);
        save(PROFILE_DATA, profile);

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

    if(message.length() >= 30) {
        message.trim();
        Serial.println("Data received.");
        Serial.println(message.length());
        Serial.println(message);
        //save(TRANSCRIPT_DATA, message);
        ble.println("smart_tag");
        Serial.flush();
        ble.flush();
    }
}

void SHIELDLib::getHealthStatus() {
    uint8_t val = pcf8574.digitalRead(P1);

	if (val == LOW) {        
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
    } else {
        if(HealthStatus == "U1") {
            displayMessage("You are", "Healthy");
        } else {
            displayMessage("You are", "Exposed");
        }
        
        delay(5000);
    }
}

/* HARDWARE COMPONENTS */

void SHIELDLib::powerOn() {
    pcf8574.pinMode(P0, OUTPUT);
    pcf8574.digitalWrite(P0, HIGH);
}

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
        //Serial.println(F("RTC missing"));
        displayError(CLOCK_MISSING);        
        //delay(50);
        return false;
    }
    return true;
}

void SHIELDLib::openBLE() {
    ble.begin(9600);
}

void SHIELDLib::initIOExpander() {
    pcf8574.begin();
    pcf8574.pinMode(P1, INPUT);     //Check Health Status Button
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

void SHIELDLib::decodeJsonData(const DeserializationError error){
    if(error) {
        Serial.print(F("Unable to decode message! Error: "));
        Serial.println(error.c_str());
    }
}

/* FILE SYSTEM */

void SHIELDLib::save(FileToSave _destinationFile, String _rawdata) {
    String __destination = getFilename(_destinationFile, ulongtoString(currentSN));

    do {
        file = SD.open(__destination, FILE_WRITE);
        /*
        if(file) {
            //Serial.println("Unable to create the file!");
            //delay(500);
        }*/
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
            _dest = dir_transcripts + _slash + "transcript_" + SequenceNumber + file_extension;
            break;

        case 4: //Dump Folder
            _dest = dir_dumps + _slash + "dumps_" + SequenceNumber + file_extension;
            break;

        case 5: //Config Folder
            _dest = dir_core + _slash + fn_CoreConfiguration + file_extension;
            break;

        case 6: //Profile Folder
            _dest = dir_profile + _slash + "profile_" + SequenceNumber + file_extension;
            break;

        case 7: //SmartTags Folder
            _dest = dir_smarttags + _slash + "smarttag_" + SequenceNumber + file_extension;
            break;
    }

    return _dest;
}

void SHIELDLib::Settings() {
    //If the Settings file is not existing, it's the device First Time Boot (FTB)
    //If existing, load settings
    //else, write default settings

    if(SD.exists(getFilename(CONFIG_DATA, ""))) {
        do {
            file = SD.open(getFilename(CONFIG_DATA, ""));
            if(!file) {
                Serial.println("Unable to load settings!");
                delay(500);
            }
        }while(!file);

        char data[500];
        int i = 0;
        while(file.available()) {
            data[i] = file.read();
            i++;
        }
        data[i] = '\0';

        file.close();

        decodeJsonData(deserializeJson(json_config, data));

        profile_interval    = json_config["PI"];
        EOS                 = json_config["EOS"];
        const char* HS      = json_config["HS"];

        HealthStatus = (String)HS;

        circ_start_time = EOS + CIRCADIAN_PERIOD;
        prof_start_time = EOS + PROFILE_PERIOD;

        Serial.println("PI:\t" + ulongtoString(profile_interval));
        Serial.println("EOS:\t" + ulongtoString(EOS));
        Serial.println("HS:\t" + HealthStatus);

        Serial.println("Settings loaded.");
    } else {
        SD.mkdir(dir_core);

        do {
            file = SD.open(getFilename(CONFIG_DATA, ""), FILE_WRITE);
            if(file) {
                Serial.println("Unable to create the settings file!");
                delay(250);
            }
        }while(!file);

        json_config["PI"]  = 15;                                    //ProfileInterval in minutes
        json_config["EOS"] = ulongtoString(getSequenceNumber());    //Epoch Time on Startup
        json_config["HS"]  = "U1";                                  //HealthStatusCode

        String rawData = "";
        serializeJson(json_config, rawData);

        file.println(rawData);
        file.close();
        
        Serial.println("Done writing default settings.");
    }
}

void SHIELDLib::changeHS(String newHS) {
    //Loads the settings first
    SD.remove(getFilename(CONFIG_DATA, ""));    //Deletes the Settings file for rewriting

    SD.mkdir(dir_core);

    do {
        file = SD.open(getFilename(CONFIG_DATA, ""), FILE_WRITE);
        if(file) {
            Serial.println("Unable to create the settings file!");
            delay(250);
        }
    }while(!file);

    json_config["PI"]  = profile_interval;                                    //ProfileInterval in minutes
    json_config["EOS"] = ulongtoString(EOS);    //Epoch Time on Startup
    json_config["HS"]  = newHS;                                  //HealthStatusCode
    HealthStatus = newHS;
    
    String rawData = "";
    serializeJson(json_config, rawData);

    file.println(rawData);
    file.close();
    
    Serial.println("Done writing default settings.");
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

String SHIELDLib::ulongtoString(uint32_t epoch) {
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