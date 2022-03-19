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
#pragma once
#ifndef SHIELDLib_H
#define SHIELDLib_H

#pragma region Libraries
#include <SD.h>             //SD Card Functionality
#include <SPI.h>            //Serial Peripheral Interface (SPI) Protocol for the SD Card Module
#include <Wire.h>           //I2C Protocol for Two-Wire modules
#include <RTClib.h>         //Real-time Clock (RTC) Functionalities
#include <PCF8574.h>        //For the PCF8574 GPIO Expander
#include <WiFiUdp.h>        //Wi-Fi User Datagram Protocol (UDP) for NTP Functionality
#include <TimeLib.h>        //Date-Time Functionality
#include <NTPClient.h>      //Network Time Protocol (NTP) Functions
#include <ESP8266WiFi.h>    //NodeMCU ESP8266 Wi-Fi Functionalities
#include <ArduinoJson.h>    //JSON in Arduino
#include <Adafruit_GFX.h>   //Use in OLED Display
#include <Adafruit_SSD1306.h>   //Use in OLED Display
#include <SoftwareSerial.h> //Serial Communication
#pragma endregion

//PIN CONFIGURATION
#define pin_csSD                D8     //SD Card Chip Select Pin
//Network Time Protocol
#define UTC_OFFSET_IN_SECONDS   28800                 //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS      "asia.pool.ntp.org"      //Network Time Protocol (NTP) Server
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
//Tag
#define TAG_VALIDITY        96  //Validity of Tags in days
#define KEY_VALIDITY        15  //Validity of Keys in minutes
#define PROFILE_VALIDITY    15  //Validity of Profile in minutes
//Close Contact Classification Period (C3P)
#define C3P_BEGIN           2   //Start of Transcript Retrieval
#define C3P_END             14  //End of Transcript Retrieval

const String dir_audit = String() + folder_Data + _slash + folder_Audit;
const String dir_circadian = String() + folder_Data + _slash + folder_Circadian;
const String dir_cirrus = String() + folder_Data + _slash + folder_CIRRUS;
const String dir_dumps = String() + folder_Data + _slash + folder_Dumps;
const String dir_memories = String() + folder_Data + _slash + folder_Memories;
const String dir_core = String() + folder_System + _slash + folder_Core;

/**
 * @brief Types of SHIELD files 
 */
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
   INIT_SD_FAILED,
   INIT_RTC_FAILED,
   INIT_OLED_FAILED
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
    U3, //Deactivated
};


/**
 * @brief SHIELD's core class.
 */
class SHIELD
{
    public:
        SHIELD();
        void startDevice();

        void sendPayload();
        void decodePayload(String _payload);

        char* getDeviceTime(DeviceTimeFormat _dtFormat);

    private:
        //Private Variables
        char* _ssid;
        char* _password;

        //File System
        void _initSD();
        String _getFilename(FileToSave _SHIELDFile);
        void _save(FileToSave _destinationFile, String _rawdata);
        void _loadSystemConfiguration();

        //OLED Display
        void _intitOLEDDisplay();

        //Clock
        void _beginClock();                  //Initializes the RTC Module
        void _syncClock();                   //Synchronizes local time with NTP Server
        void _displayDateTime();             //Displays Date and Time in the OLED Display
        unsigned long _getTimestamp();       //Provides the Unix-format Timestamp
        unsigned long _getNTPUnixTime();    //Gets the NTP Unix time
        
        //Wi-Fi
        void _connecttoWifi();

        //Encryption
        String _encrypt(String _rawData);
        String _decrypt(String _rawData);

        //Others
        void _audioNotify();
};
#endif