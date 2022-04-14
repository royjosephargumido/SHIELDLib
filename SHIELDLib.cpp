#include "SHIELDLib.h"

RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);
Adafruit_SSD1306 deviceOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void SHIELDLib::startDevice() {
    shield.initOLED();      // Initializes the OLED display
    shield.initSDCard();    // Initializes the SD Card module
    shield.beginClock();    // Begins the clock
    
    shield.syncClock();
    
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

char* SHIELDLib::getSequenceNumber() {
    char* _dt = (char*)malloc(50);
    DateTime now = rtc.now();
    unsigned long _unixtime = now.unixtime();

    ltoa(_unixtime, _dt, 10);
    return _dt;
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
        displayError(SD_MISSING);
        delay(50);
    }
}

bool SHIELDLib::beginClock() {
    while(!rtc.begin()) {
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
        String min_sec = (String)':' + Min + ':' + Sec;

        String Date =  (String)now.month() + '/' + (String)now.day() + '/' + now.year();
        String Time = (now.hour() > 12) ? (String)(now.hour() % 12) + min_sec + " PM" : (String)now.hour() + min_sec + " AM";

        char _time[12];
        char _date[11];
        Time.toCharArray(_time, 11);
        Date.toCharArray(_date, 10);

        lastsaved_time = Time;
        lastsaved_date = Date;

        displayMessage(_time, _date);
    }
}

void SHIELDLib::syncClock() {
    DateTime now = rtc.now();
    int _unixtime = now.unixtime();

    if(_unixtime < 1649721600) {
        displayMessage("Clock", "Syncing...");

        shield.connecttoWIFI("ODIMUGRA", "odimugra023026");
        timeClient.begin();

        int _timer = 0;
        unsigned long _t;
        do {
            timeClient.update();
            _t = timeClient.getEpochTime(); //Connects to the NTP Server and gets Unix time
            _timer += 1;
        }while(_timer < 2);

        rtc.adjust(DateTime(year(_t), month(_t), day(_t), hour(_t), minute(_t), second(_t)));
        WiFi.disconnect();
    }
}

void SHIELDLib::connecttoWIFI(char* wifi_ssid, char* wifi_password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED){}
}

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
    for(int index = 0; index < 32; pos++, ++index) {
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