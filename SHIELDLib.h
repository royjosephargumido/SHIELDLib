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

#include <SD.h>             //SD Card Functionality
#include <SPI.h>            //Serial Peripheral Interface (SPI) Protocol for the SD Card Module
#include <Wire.h>           //I2C Protocol for Two-Wire modules
#include <RTClib.h>         //Real-time Clock (RTC) Functionalities
#include <WiFiUdp.h>        //Wi-Fi User Datagram Protocol (UDP) for NTP Functionality
#include <TimeLib.h>        //Date-Time Functionality
#include "base64.hpp"       //Base64 functionality
#include <NTPClient.h>      //Network Time Protocol (NTP) Functions
#include <ESP8266WiFi.h>    //NodeMCU ESP8266 Wi-Fi Functionalities
#include <ArduinoJson.h>    //JSON in Arduino

#define UTC_OFFSET_IN_SECONDS 28800                 //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS "asia.pool.ntp.org"      //Network Time Protocol (NTP) Server
#define TAG_EXPIRATION_TIME 15                      //Tag expiration time in minutes

/**
 * @brief SHIELD Device Types
 */
enum DeviceType {
    BEACON,
    NEURON
} currentType;

/**
 * @brief SHIELD Timestamp Formats
 * 
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
    U5, //System Flag - General Flag
    U6  //System Flag - Deactivated
};

/**
 * @brief Used for displaying messages
 */
enum DisplayChannel {
    useSystemDefault,
    useSerialMonitor,
    useOLEDDisplay,
    useBothDisplays
} currentDisplayChannel;

class Device
{
    public:
        Device(DeviceType _deviceAs);
        void startDevice();
        const char* getDeviceType();
        char* getDeviceTime(DeviceTimeFormat _dtFormat);

    private:
};

/**
 * @brief SHIELD Clock Module
 * 
 */
class Clock
{
    public:
        void beginClock();                  //Initializes the RTC Module
        void syncClock();                    //Synchronizes local time with NTP Server
        unsigned long getTimestamp();       //Provides the Unix-format Timestamp
        
    private:
        unsigned long _getNTPUnixTime();    //Gets the NTP Unix time
        
};

class Wifi
{
    public:
        void connect(char* wifi_ssid, char* wifi_password);
        
    private:
        char* _ssid;
        char* _password;        
};

class Tag
{
    public:
        void getHealthStatus();
        void getCIRRUSVersion();
        char* createGUID();
        char* getContactNumber();
        char* getSensorData();        

    private:
        void setHealthStatus(HealthStatus _hstatus);
        HealthStatus currentHealthStatus;
};

#endif