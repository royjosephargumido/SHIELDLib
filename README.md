# Arduino Library for SHIELD

This is an ESP8266 library for **Smart Tag-based Human Infection Reporting and Encounter Logging Device (SHIELD)**.
The primary goal of this library is to easily implement SHIELD functionality.

## Library

To use the SHIELD library in an Arduino sketch, include SHIELDLib.h.

```c
#include <SHIELDLib.h>
```

## Functionality
SHIELD implements four core funtionalities:

```c
//placed on the setup()
shield.startDevice();       //Initializes and begins the SHIELD
```

```c
//placed on the loop()
shield.displayDateTime();   //Displays the Date and Time on the OLED
shield.protocolbegin();     //Begins the SHIELD Protocol and Cryptography Modules
shield.listen();            //Sends profile and recieves transcript to and from other devices
```

## File System
SHIELD strictly follow the following file structuring schemes:

**Beacon File Structuring**


    FileSystem:
    |
    +---Data
    |   |
    |   +--- Audit
    |   |    |
    |   |    +--- audit_1642084941.shield
    |   |
    |   +--- Circadian
    |   |    |
    |   |    +--- circ_1642084941.shield
    |   |
    |   +--- CIRRUS
    |   |    |
    |   |    +--- cirrus.shield
    |   |
    |   +--- Memories
    |   |     |
    |   |     +--- transcript_1642084941.shield
    |   +--- Profile
    |   |    |
    |   |    +--- profile_1642084941.shield
    |   |
    |   +--- Memories
    |   |    |
    |   |    +--- transcript_1642084941.shield
    |   |
    |   +--- SmartTags
    |        |
    |        +--- smarttag_1642084941.shield
    |   
    +---System
        |
        +--- Documentation
        |    |
        |    +--- SHIELD User Manual.pdf
        |
        +--- Dumps
        |    |
        |    +--- dump_1642084941.shield
        |
        +--- Core
            |
            +--- beaconconfig.shield
            |
            +--- wifi.shield

## Required Third-Party Libraries
SHIELD uses numerous third-party libraries to perform specific functions. These libraries are required and must be installed prior before using the SHIELD library.

| **Library** | **Description** | **Version** |
| ----------- | --------------- | ----------- |
| SD.h | SD Card Functionality. | 1.2.4 |
| SPI.h | Serial Peripheral Interface (SPI) Protocol for the SD Card Module. | *Builtin* |
| Wire.h | I2C Protocol for Two-Wire modules. | *Builtin* |
| RTClib.h | Real-time Clock (RTC) functionalities. | 2.0.2 |
| FastLED.h | WS2811 RGB Led | |
| PCF8574.h | PCF8574 GPIO Expander | |
| WiFiUdp.h | Wi-Fi User Datagram Protocol (UDP) for NTP Functionality. | *Builtin* |
| TimeLib.h | Date-Time related functionalities. | 1.6.1 |
| base64.hpp | Base64 functionality | |
| NTPClient.h |Network Time Protocol (NTP) functionalities. | 3.2.0 |
| ESP8266WiFi.h | NodeMCU ESP8266 Wi-Fi functionalities. | *Builtin* |
| ArduinoJson.h | JSON in Arduino. | 6.19.1 |
| SoftwareSerial.h | Serial Communication | |

[1]:<https://github.com/rjargumido/SHIELD>
