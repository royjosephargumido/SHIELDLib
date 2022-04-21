#include "SHIELDLib.h"

File file;
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);
Adafruit_SSD1306 deviceOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Cryptography
uint32_t prof_start_time    = 0;
uint32_t profile_interval   = 1;
const uint32_t PROFILE_PERIOD     = profile_interval * 60;

uint32_t circ_start_time    = 0;
const uint32_t CIRCADIAN_PERIOD   = 2 * 60;   //In seconds

uint32_t currentSN = 0;
bool ftb = true;


void SHIELDLib::startDevice() {
    shield.initOLED();      // Initializes the OLED display
    shield.initSDCard();    // Initializes the SD Card module
    shield.beginClock();    // Begins the clock
    
    syncClock();

    uint32_t currSN = getSequenceNumber();
    circ_start_time = currSN + CIRCADIAN_PERIOD;
    prof_start_time = currSN + PROFILE_PERIOD;
}

char* _dt = (char*)malloc(50);

void SHIELDLib::protocolbegin() {
    currentSN = getSequenceNumber();
    
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

        if(!ftb) {
            circ_start_time += CIRCADIAN_PERIOD;
        }
    }    
    
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
        String cipher = encrypt(CIRCADIAN, PUK, TUK);               // Encrypts PUK using CIRCADIAN and TUK in AES128-CTR
        //uint32_t CV = getCIRRUSVersion();
        cipher = ulongtoString(currentSN) + '-' + cipher;

        // Encodes the ciphertext to Base64
        unsigned char smart_tag[61];
        // encode_base64() places a null terminator automatically, because the output is a string
        unsigned int base64_length = encode_base64((unsigned char*)cipher.c_str(), strlen(cipher.c_str()) + 1, smart_tag);
        
        sprintf(_dt, "%02d/%02d/%02d %02d:%02d:%02d", day(currentSN), month(currentSN), year(currentSN), hour(currentSN), minute(currentSN), second(currentSN));
        Serial.println(_dt);
        Serial.println("=========================================");
        Serial.print("Smart Tag:\n");
        Serial.println((char *) smart_tag);
        Serial.println("=========================================");

        /*
        Serial.println("PUK:\t" + puk);
        Serial.println("TUK:\t" + tuk);
        Serial.println("PID:\t" + pid);
        Serial.println("=========================================");
        Serial.println("Profile:\n" + cipher);

        // Decoding base64
        char rawdata[32];
        decode_base64(smart_tag, (unsigned char*)rawdata);
        Serial.print("Rawdata:\n");
        Serial.println((char *) rawdata);
        Serial.println("=========================================");
        char* _cip = (char*)malloc(32);
        sprintf(_cip, "%s", cipher);
        Serial.print("Status: ");
        if (strcmp(_cip, rawdata) == 0)
            Serial.println("Passed.");
        else
            Serial.println("Passed.");
        */
        Serial.println();
        Serial.println();
          
        save(PROFILE_DATA, (char*)smart_tag);

        if(!ftb) {
            prof_start_time += PROFILE_PERIOD;
        }
    }

    if(ftb) {
        ftb = false;
    }
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

String SHIELDLib::ulongtoString(uint32_t epoch) {
    char* _dt = (char*)malloc(50);
    ltoa(epoch, _dt, 10);
    return String(_dt);
}

uint32_t SHIELDLib::getSequenceNumber() {
    DateTime now = rtc.now();
    return now.unixtime();
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
        Serial.println(F("RTC missing"));
        displayError(CLOCK_MISSING);        
        delay(50);
        return false;
    }
    return true;
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

const char *ssid     = "ODIMUGRA";
const char *password = "odimugra023026";

void SHIELDLib::syncClock() {
    DateTime now = rtc.now();
    uint32_t _unixtime = now.unixtime();

    if(_unixtime < 1649721600) {
        displayMessage("Clock", "Syncing...");

        WiFi.begin(ssid, password);

        Serial.println("Connecting...");
        while ( WiFi.status() != WL_CONNECTED ) {
            delay ( 500 );
        }
        Serial.println("Connected.");

        timeClient.begin();

        timeClient.update();
        unsigned long _t = timeClient.getEpochTime();

        rtc.adjust(DateTime(year(_t), month(_t), day(_t), hour(_t), minute(_t), second(_t)));
        WiFi.disconnect();
    }
}

void SHIELDLib::connecttoWIFI(char* wifi_ssid, char* wifi_password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED){}
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

void SHIELDLib::save(FileToSave _destinationFile, String _rawdata) {
    String __destination = getFilename(_destinationFile, ulongtoString(getSequenceNumber()));

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

/* SHIELD's CORE FUNCTIONS */

/* SHIELD'S CRYPTOGRAPHY FUNCTIONS */

HKDF<SHA256> hkdf_hmac_object;
CTR<AES128> ctraes128;

ICACHE_FLASH_ATTR SHIELDLib::SHIELDLib() {
	lastYield = 0;
}

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

unsigned char SHIELDLib::binary_to_base64(unsigned char v) {
    // Capital letters - 'A' is ascii 65 and bas88e64 0
    if(v < 26) return v + 'A';

    // Lowercase letters - 'a' is ascii 97 and base64 26
    if(v < 52) return v + 71;

    // Digits - '0' is ascii 48 and base64 52
    if(v < 62) return v - 4;

    // '+' is ascii 43 and base64 62
    if(v == 62) return '+';

    // '/' is ascii 47 and base64 63
    if(v == 63) return '/';

    return 64;
}

unsigned int SHIELDLib::encode_base64_length(unsigned int input_length) {
    return (input_length + 2)/3*4;
}

unsigned int SHIELDLib::encode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]) {
    unsigned int full_sets = input_length/3;

    // While there are still full sets of 24 bits...
    for(unsigned int i = 0; i < full_sets; ++i) {
        output[0] = binary_to_base64(                         input[0] >> 2);
        output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
        output[2] = binary_to_base64((input[1] & 0x0F) << 2 | input[2] >> 6);
        output[3] = binary_to_base64( input[2] & 0x3F);

        input += 3;
        output += 4;
    }

    switch(input_length % 3) {
        case 0:
            output[0] = '\0';
            break;
        case 1:
            output[0] = binary_to_base64(                         input[0] >> 2);
            output[1] = binary_to_base64((input[0] & 0x03) << 4);
            output[2] = '=';
            output[3] = '=';
            output[4] = '\0';
            break;
        case 2:
            output[0] = binary_to_base64(                         input[0] >> 2);
            output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
            output[2] = binary_to_base64((input[1] & 0x0F) << 2);
            output[3] = '=';
            output[4] = '\0';
            break;
    }

    return encode_base64_length(input_length);
}

unsigned char SHIELDLib::base64_to_binary(unsigned char c) {
    // Capital letters - 'A' is ascii 65 and base64 0
    if('A' <= c && c <= 'Z') return c - 'A';

    // Lowercase letters - 'a' is ascii 97 and base64 26
    if('a' <= c && c <= 'z') return c - 71;

    // Digits - '0' is ascii 48 and base64 52
    if('0' <= c && c <= '9') return c + 4;

    // '+' is ascii 43 and base64 62
    if(c == '+') return 62;

    // '/' is ascii 47 and base64 63
    if(c == '/') return 63;

    return 255;
}

unsigned int SHIELDLib::decode_base64_length(unsigned char input[], unsigned int input_length) {
    unsigned char *start = input;

    while(base64_to_binary(input[0]) < 64 && (unsigned int) (input - start) < input_length) {
        ++input;
    }

    input_length = (unsigned int) (input - start);
    return input_length/4*3 + (input_length % 4 ? input_length % 4 - 1 : 0);
}

unsigned int SHIELDLib::decode_base64_length(unsigned char input[]) {
    return decode_base64_length(input, -1);
}

unsigned int SHIELDLib::decode_base64(unsigned char input[], unsigned char output[]) {
    return decode_base64(input, -1, output);
}

unsigned int SHIELDLib::decode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]) {
    unsigned int output_length = decode_base64_length(input, input_length);

    // While there are still full sets of 24 bits...
    for(unsigned int i = 2; i < output_length; i += 3) {
        output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
        output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
        output[2] = base64_to_binary(input[2]) << 6 | base64_to_binary(input[3]);

        input += 4;
        output += 3;
    }

    switch(output_length % 3) {
        case 1:
            output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
            break;
        case 2:
            output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
            output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
            break;
    }

    return output_length;
}

SHIELDLib shield;
