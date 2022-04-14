#ifndef SHIELDLIB_h
#define SHIELDLIB_h

#include "src/Crypto.h"
#include "src/SHA256.h"
#include "src/HKDF.h"
#include "src/AES.h"
#include "src/CTR.h"

#include <Arduino.h>
#include <inttypes.h>

#include <SD.h>                 //SD Card Functionality
#include <SPI.h>                //Serial Peripheral Interface (SPI) Protocol for the SD Card Module
#include <Wire.h>               //I2C Protocol for Two-Wire modules
#include <RTClib.h>             //Real-time Clock (RTC) Functionalities
//#include <PCF8574.h>            //For the PCF8574 GPIO Expander
#include <WiFiUdp.h>            //Wi-Fi User Datagram Protocol (UDP) for NTP Functionality
#include <TimeLib.h>            //Date-Time Functionality
#include <NTPClient.h>          //Network Time Protocol (NTP) Functions
#include <ESP8266WiFi.h>        //NodeMCU ESP8266 Wi-Fi Functionalities
//#include <ArduinoJson.h>        //JSON in Arduino
#include <Adafruit_GFX.h>       //Use in OLED Display
#include <Adafruit_SSD1306.h>   //Use in OLED Display
#include <SoftwareSerial.h>     //Serial Communication

#define MAX_BLOCKS              16                      //Number of bytes
#define pin_csSD                D8                      //SD Card Chip Select Pin
//Network Time Protocol
#define UTC_OFFSET_IN_SECONDS   28800                   //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS      "asia.pool.ntp.org"     //Network Time Protocol (NTP) Server
//File System
#define _slash                   '/'
#define file_extension          ".shield"
#define folder_Data             "Data"
#define folder_Audit            "Audit"
#define folder_Circadian        "Circadian"
#define folder_CIRRUS           "CIRRUS"
#define folder_Memories         "Memories"
#define folder_System           "System"
#define folder_Core             "Core"
#define folder_Docu             "Documentation"
#define folder_Dumps            "Dumps"
#define fn_CoreConfiguration    "BeaconConfig"
//Baud
#define baud_rate           115200
//OLED
#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT       32  // OLED display height, in pixels

const String dir_audit = String() + folder_Data + _slash + folder_Audit;
const String dir_circadian = String() + folder_Data + _slash + folder_Circadian;
const String dir_cirrus = String() + folder_Data + _slash + folder_CIRRUS;
const String dir_dumps = String() + folder_Data + _slash + folder_Dumps;
const String dir_memories = String() + folder_Data + _slash + folder_Memories;
const String dir_core = String() + folder_System + _slash + folder_Core;

enum FileToSave {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA,
    PROFILE_DATA
};

enum ErrorCodes {
    CLOCK_MISSING,
    SD_MISSING,
};

// HKDF INFO
unsigned char const INFO_PUK[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X50, 0X55, 0X4b};    //SHIELD-PUK
unsigned char const INFO_TUK[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X54, 0X55, 0X4b};    //SHIELD-TUK
unsigned char const INFO_PID[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X50, 0X49, 0X44};    //SHIELD-PID

class SHIELDLib {
    public:
        void startDevice();
        char* getSequenceNumber();
        void displayDateTime();

        /* SHIELD'S CRYPTOGRAPHY FUNCTIONS */

        //Converts a byte array to its hexadecima/string representation
        String bytetostring(byte array[]);

        //Converts a string to a byte array
        void stringtobyte(uint8_t* location, String rawdata);

        //TrueRandom Number Generator (TRNG)
        ICACHE_FLASH_ATTR SHIELDLib();	
        ICACHE_FLASH_ATTR String trng(uint8_t* location, int outputLength);

        // Hash Key Derivation Function based on HMAC-SHA256
        String perfHKDF(const unsigned char *key, const unsigned char *salt, const unsigned char *info, size_t outputlength);

        // AES128-CTR (Advanced Encryption Standard - 128 bits on Counter Mode)
        String encrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce);
        String decrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce);

        //Base64
        unsigned int encode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]);
        unsigned int decode_base64(unsigned char input[], unsigned char output[]);
        
    private:
        String lastsaved_time;
        String lastsaved_date;
        
        void initOLED();
        void initSDCard();
        bool beginClock();

        void syncClock();
        void connecttoWIFI(char* wifi_ssid, char* wifi_password);

        void displayError(ErrorCodes err);
        void displayMessage(char* line1, char* line2);

        /* SHIELD'S CRYPTOGRAPHY FUNCTIONS */

        //TrueRandom Number Generator (TRNG)
        unsigned long lastYield;
        ICACHE_FLASH_ATTR int randomBit();              // Generates a random bit
        ICACHE_FLASH_ATTR int whiten();                 // Software whiten the generated random bit
        ICACHE_FLASH_ATTR char randomByte();            // Generates a random byte

        //Base64 Encoding required functions
        unsigned char binary_to_base64(unsigned char v);
        unsigned int encode_base64_length(unsigned int input_length);

        //Base64 Decoding required functions
        unsigned char base64_to_binary(unsigned char c);
        unsigned int decode_base64_length(unsigned char input[]);
        unsigned int decode_base64_length(unsigned char input[], unsigned int input_length);
        unsigned int decode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]);
};

extern SHIELDLib shield;
#endif
