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
syncClock()                                     //Synchronizes device clock with NTP Server
```

**Note:** ***There is no need to call or invoke a certain functionality from other system module since all other system functions are automatically managed by the Device constructor upon creation.***

## System Modules
SHIELD runs in an interconnected functions defined by its respective classes called **modules**.

**Hardware-involved Modules:**

- `Clock` manages the Date and Time function of the system.
  **Hardware:** `DS3231 RTC Module`

- `Wifi` manages the Wi-Fi capability of the system.
  **Hardware:** `NodeMCU v3 ESP8266 board`

- `BLE` manages the Blowtooth Low Energy (BLE) functions.
  **Hardware:** `AT-09 BLE Module with Bluetooth 4.0 CC2541 BLE Chip`

- `Datalogger` serves as the system's file manager.
  **Hardware:**
    - `MicroSD Card at least 4GB (12GB MAX)`
    - `MicroSD Card Module`

- `Display` manages the visual output of the system (both serial and OLED displays).
    **Hardware:** `0.91-inch OLED 128X32 display module`

- `Notification` handles the audio and visual notification functions.
  **Hardware:**
    - `3V Active Buzzer`
    - `WS2812B 5050 SMD RGB LED with WS2811 LED controller chip`

- `Sensor` manages the Gyroscope component.
    **Hardware:** `GY-521 MPU-6050 Accelerometer + Gyroscope Module`

- `Touch` manages the touch switch component.
    **Hardware:** `TTP223 Key Switch Module Button`


**Software-only Modules:**

- `Protocol` performs the overall contact tracing protocol of SHIELD.
- `AES` handles the AES cryptography methods.
- `Data` handles the data preprocessing. 

## Clock Module
SHIELD uses a DS3231 Realtime Clock (RTC) module to provide the Date and Time Functionalities. By default, it provides the timestamp in **GMT+08:00 timezone** as specified by the `UTC_OFFSET_IN_SECONDS` macro.

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
| ArduinoJson.h | JSON in Arduino. | 6.19.1 |

[1]:<https://github.com/rjargumido/SHIELD>