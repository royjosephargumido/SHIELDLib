#ifndef SHIELDLIB_h
#define SHIELDLIB_h

#include <Arduino.h>
#include <inttypes.h>

#include <SD.h>                 //SD Card Functionality
#include <SPI.h>                //Serial Peripheral Interface (SPI) Protocol for the SD Card Module
#include <Wire.h>               //I2C Protocol for Two-Wire modules
#include <RTClib.h>             //Real-time Clock (RTC) Functionalities
#include <WiFiUdp.h>            //Wi-Fi User Datagram Protocol (UDP) for NTP Functionality
#include <TimeLib.h>            //Date-Time Functionality
#include <NTPClient.h>          //Network Time Protocol (NTP) Functions
#include <WiFiClient.h>         //NodeMCU as a WiFi access point
#include <ESP8266WiFi.h>        //NodeMCU ESP8266 Wi-Fi Functionalities
#include <ArduinoJson.h>        //JSON in Arduino
#include <Adafruit_GFX.h>       //Use in OLED Display
#include <SoftwareSerial.h>     //Serial Communication
#include <Adafruit_SSD1306.h>   //Use in OLED Display
#include <ESP8266WebServer.h>   //Web Server functionalities

#include "src/Crypto.h"
#include "src/SHA256.h"
#include "src/HKDF.h"
#include "src/AES.h"
#include "src/CTR.h"

#include "src/SHIELDHTMLPayload.h"

// Utilities
#define MAX_BLOCKS              16                      //Number of bytes
#define pin_csSD                D8                      //SD Card Chip Select Pin
#define baud_rate               115200                  //Default Baud
#define SCREEN_WIDTH            128                     // OLED display width, in pixels
#define SCREEN_HEIGHT           32                      // OLED display height, in pixels
#define UTC_OFFSET_IN_SECONDS   28800                   //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS      "asia.pool.ntp.org"     //Network Time Protocol (NTP) Server

//File System
#define _slash                   '/'
#define file_extension          ".shield"
#define folder_Data             "Data"
#define folder_Audit            "Audit"
#define folder_Circadian        "Circadian"
#define folder_CIRRUS           "CIRRUS"
#define folder_Transcripts      "Transcripts"
#define folder_Profile          "Profile"
#define folder_SmartTag         "SmartTags"
#define folder_System           "System"
#define folder_Core             "Core"
#define folder_Dumps            "Dumps"
#define fn_CoreConfiguration    "SHIELDConfig"

const String dir_audit          = String() + folder_Data + _slash + folder_Audit;
const String dir_circadian      = String() + folder_Data + _slash + folder_Circadian;
const String dir_cirrus         = String() + folder_Data + _slash + folder_CIRRUS;
const String dir_dumps          = String() + folder_Data + _slash + folder_Dumps;
const String dir_transcripts    = String() + folder_Data + _slash + folder_Transcripts;
const String dir_smarttags      = String() + folder_Data + _slash + folder_SmartTag;
const String dir_core           = String() + folder_System + _slash + folder_Core;
const String dir_profile        = String() + folder_Data + _slash + folder_Profile;

enum FileToSave {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA,
    PROFILE_DATA,
    SMARTTAG_DATA
};

enum ErrorCodes {
    CLOCK_MISSING,
    SD_MISSING,
};

// Web Server
bool isloggedin();
void handleFTB();
void route_Root();
void route_Login();
void route_Activate();
void route_Faqs();
void route_Main();

void loadSettings();
void writeSettings();
String getFilename(FileToSave _SHIELDFile, String SequenceNumber);

void Settings();
bool getStatus(String message);
void changeHS();

class SHIELDLib {
    public:
        //Constructor
        ICACHE_FLASH_ATTR SHIELDLib();

        //Core functionalities
        void startDevice();
        void beginWebServer();
        void protocolbegin();
        void listen();
        void getExposureStatus();
        void handleWebServer();
        void startSHIELD();

        void decodeJsonData(const DeserializationError error);

        // Utility
        void syncClock();
        void connecttoWIFI(char* wifi_ssid, char* wifi_password);
        void displayError(ErrorCodes err);
        void displayMessage(char* line1, char* line2);

        // File System
        void save(FileToSave _destinationFile, String _rawdata);

        /* SHIELD'S CRYPTOGRAPHY FUNCTIONS */

        // Core Cryptography Functions

        // SequenceNumber
        uint32_t getSequenceNumber();
        // TrueRandom Number Generator (TRNG)
        ICACHE_FLASH_ATTR String trng(uint8_t* location, int outputLength);
        // Hash Key Derivation Function based on HMAC-SHA256
        String perfHKDF(const unsigned char *key, const unsigned char *salt, const unsigned char *info, size_t outputlength);
        // AES128-CTR (Advanced Encryption Standard - 128 bits on Counter Mode)
        String encrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce);
        String decrypt(const unsigned char *key, const unsigned char *data, const unsigned char *nonce);

        // Cryptography Utilities

        // SequenceNumber
        String ulongtoString(uint32_t epoch);                   //Converts uint32_t SequenceNumber to its String representation
        String bytetostring(byte array[]);                      //Converts a byte array to its hexadecima/string representation
        void stringtobyte(uint8_t* location, String rawdata);   //Converts a string to a byte array

        // TrueRandom Number Generator (TRNG)
        unsigned long lastYield;
        ICACHE_FLASH_ATTR int randomBit();              // Generates a random bit
        ICACHE_FLASH_ATTR int whiten();                 // Software whiten the generated random bit
        ICACHE_FLASH_ATTR char randomByte();            // Generates a random byte

    private:
        //SSID and Password of your WiFi router
        const char* ssid			= "SHIELD";
        const char* password		= "1234567890";

        byte CIRCADIAN[MAX_BLOCKS];   // Circadian
        // HKDF INFO
        unsigned char const INFO_PUK[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X50, 0X55, 0X4b};    //SHIELD-PUK
        unsigned char const INFO_TUK[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X54, 0X55, 0X4b};    //SHIELD-TUK
        unsigned char const INFO_PID[10] = {0X53, 0X48, 0X49, 0X45, 0X4c, 0X44, 0X2d, 0X50, 0X49, 0X44};    //SHIELD-PID
        
        // Cryptography

        // Hardware components
        void powerOn();
        void initOLED();
        void initSDCard();
        bool beginClock();
        void openBLE();
        void initButton();
};

extern SHIELDLib shield;
#endif