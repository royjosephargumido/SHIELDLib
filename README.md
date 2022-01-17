# Arduino Library for SHIELD

This is an Arduino library for [Smart Tag-based Human Infection Reporting and Encounter Logging Device (SHIELD)][1].
The primary goal of this library is to easily implement SHIELD device functionality.

## SHIELD Devices
SHIELD comprises of 2 types of device:

1. **Beacon** which can be invoked by using the macro `BEACON`.
2. **Neuron** which can be invoked by using the macro `NEURON`.

## Functionality

To use the SHIELD library in an Arduino sketch, include SHIELDLib.h.

```c
#include <SHIELDLib.h>
```

Then create the device constructor:

```c
Device device_name(device_type);
```
Replace `device_name` with your identifier and `device_type` with the device type macro *BEACON* or *NEURON*. The library automatically provide all applicable functions easily.

After setting the device'object, the following are the base functions availble for the two device:

```c
startDevice()                                   //Starts the device.
getDeviceType()                                 //Retrieves the device type.
getDeviceTime(DeviceTimeFormat _dtFormat)       //Retrieves device time in a specified system formatting.
```

## Required Third-Party Libraries
SHIELD uses numerous third-party libraries to perform specific functions. These libraries are required and must be installed prior before using the SHIELD library.

| **Library** | **Description** | **Version** |
| ----------- | --------------- | ----------- |
| SD.h | SD Card Functionality. | 1.2.4 |
| SPI.h | Serial Peripheral Interface (SPI) Protocol for the SD Card Module. | *Builtin* |
| Wire.h | I2C Protocol for Two-Wire modules. | *Builtin* |
| RTClib.h | Real-time Clock (RTC) functionalities. | 2.0.2 |
| WiFiUdp.h | Wi-Fi User Datagram Protocol (UDP) for NTP Functionality. | *Builtin* |
| TimeLib.h | Date-Time related functionalities. | 1.6.1 |
| NTPClient.h |Network Time Protocol (NTP) functionalities. | 3.2.0 |
| ESP8266WiFi.h | NodeMCU ESP8266 Wi-Fi functionalities. | *Builtin* |
| ArduinoJson.h | /JSON in Arduino. | 6.19.1 |

[1]:<https://github.com/rjargumido/SHIELD>