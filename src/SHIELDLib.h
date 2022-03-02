/**
 * @file SHIELDLib.h
 * @author Roy Joseph Argumido (royjosephargumido@outlook.com)
 * @brief Library for Smart Tag-based Human Infection Reporting and Encounter Logging Device (SHIELD).
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 * LICENSE: GNU Affero General Public License v3.0
 */

//Include guards to prevent from  multiple inclusion/declaration of the same header file.
#ifndef SHIELDLib_H
#define SHIELDLib_H

#pragma region Libraries
#include <SD.h>             //SD Card Functionality
#include <SPI.h>            //Serial Peripheral Interface (SPI) Protocol for the SD Card Module
#include <Wire.h>           //I2C Protocol for Two-Wire modules
#include <RTClib.h>         //Real-time Clock (RTC) Functionalities
#include <FastLED.h>        //WS2811 Led Notification
#include <PCF8574.h>        //For the PCF8574 GPIO Expander
#include <WiFiUdp.h>        //Wi-Fi User Datagram Protocol (UDP) for NTP Functionality
#include <TimeLib.h>        //Date-Time Functionality
#include "base64.hpp"       //Base64 functionality
#include <NTPClient.h>      //Network Time Protocol (NTP) Functions
#include <ESP8266WiFi.h>    //NodeMCU ESP8266 Wi-Fi Functionalities
#include <ArduinoJson.h>    //JSON in Arduino
#include <Adafruit_GFX.h>   //Use in OLED Display
#include <Adafruit_SSD1306.h>   //Use in OLED Display
#include <SoftwareSerial.h> //Serial Communication
#pragma endregion

#pragma region PIN Configuration
#define pin_dataLED D2  //Data Pin used by the LED
#define pin_csSD D8     //SD Card Chip Select Pin
#pragma endregion

#pragma region NTP
#define UTC_OFFSET_IN_SECONDS 28800                 //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS "asia.pool.ntp.org"      //Network Time Protocol (NTP) Server
#pragma endregion

#pragma region File System
#define slash '/'
#define file_extension ".shield"

#define folder_Data "Data"
#define folder_Audit "Audit"
#define folder_Circadian "Circadian"
#define folder_CIRRUS "CIRRUS"
#define folder_Memories "Memories"

#define folder_System "System"
#define folder_Core "Core"
#define folder_Docu "Documentation"
#define folder_Dumps "Dumps"

#define fn_CoreConfiguration "BeaconConfig"

const String dir_audit = String() + folder_Data + slash + folder_Audit + slash;
const String dir_circadian = String() + folder_Data + slash + folder_Circadian + slash;
const String dir_cirrus = String() + folder_Data + slash + folder_CIRRUS + slash;
const String dir_memories = String() + folder_Data + slash + folder_Memories + slash;
const String dir_memories = String() + folder_Data + slash + folder_Dumps + slash;
const String dir_core = String() + folder_System + slash + folder_Core + slash + fn_CoreConfiguration + file_extension;
#pragma endregion

#pragma region Others
CRGB leds[1];
#define baud_rate 115200

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#pragma endregion

/**
 * @brief Types of SHIELD files 
 */
enum FileToSave {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA
};

/**
 * @brief SHIELD Timestamp Formats
 */
enum DeviceTimeFormat {
    inUnix,
    inHumanReadableFormat
};

/**
 * @brief SHIELD Health Status 
 */
enum HealthStatus {
    U0, //Healthy or Normal
    U1, //Suspected
    U2, //Positive
    U3, //Recovered
    U4, //Dead
    U5, //System Flag - Deactivated
    U6  //System Flag - General Flag
};

/**
 * @brief Used for displaying messages
 */
enum DisplayChannel {
    useBothDisplays, /*SYSTEM DEFAULT DISPLAY*/
    useSerialMonitor,
    useOLEDDisplay
} currentDisplayChannel;

void display(String _message);

class SHIELDDevice
{
    public:
        SHIELDDevice(DeviceType _setdeviceAs);
        void startDevice();

        void sendPayload();
        void decodePayload(String _payload);

        char* getDeviceTime(DeviceTimeFormat _dtFormat);
        void setDisplayChannel(DisplayChannel _dchannel);

    private:
        //Private Variables
        char* _ssid;
        char* _password;

        //File System
        void _initSD();
        void _save(FileToSave _filetosave, String _rawdata);
        void _loadSystemConfiguration();

        //OLED Display
        void _intitOLEDDisplay();

        //Clock
        void _beginClock();                  //Initializes the RTC Module
        void _syncClock();                   //Synchronizes local time with NTP Server
        unsigned long _getTimestamp();       //Provides the Unix-format Timestamp
        unsigned long _getNTPUnixTime();    //Gets the NTP Unix time
        
        //Wi-Fi
        void _connect(char* wifi_ssid, char* wifi_password);

        //Encryption
        String _encrypt(String _rawData);
        String _decrypt(String _rawData);

        //Others
        void _setbaudrate();
        void _lightupLED();
};
#endif