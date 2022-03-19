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

#include "SHIELDDependents.h"

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