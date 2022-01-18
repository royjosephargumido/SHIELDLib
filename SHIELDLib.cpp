#include "SHIELDLib.h"

RTC_DS3231 rtc;
WiFiUDP ntpUDP;

Clock _clk;
Wifi _wf;

NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);

#pragma region Device
/**
 * @brief Initializes the device type which automatically configures the device based on its type. 
 * 
 * @param _deviceAs Set the device type either as BEACON or as NEURON.
 */
Device::Device(DeviceType _setdeviceAs) {
    currentType = _setdeviceAs;
}

/**
 * @brief Starts the device based on the set device type. 
 */
void Device::startDevice() {
    _clk.beginClock();    
    _clk.syncClock();

    Serial.print("SHIELD Device ");
    Serial.print(getDeviceType());
    Serial.println(" now running.");
}

/**
 * @brief Returns the SHIELD device type.
 * 
 * @return const char* 
 */
const char* Device::getDeviceType() {
    const char* type = (currentType == BEACON) ? "Beacon" : "Neuron";
    return type;
}

/**
 * @brief Returns device timestamp based on the specifie format.
 * 
 * @param _dtFormat 
 * @return char* 
 */
char* Device::getDeviceTime(DeviceTimeFormat _dtFormat) {
    char* _dt = (char*)malloc(50);
    DateTime now = rtc.now();
    unsigned long _unixtime = now.unixtime();

    switch(_dtFormat){
        case inUnix:            
            ltoa(_unixtime, _dt, 10);
            break;
        
        case inHumanReadableFormat:
            sprintf(_dt, "%02d.%02d.%02d %02d:%02d:%02d", day(_unixtime), month(_unixtime), year(_unixtime), hour(_unixtime), minute(_unixtime), second(_unixtime));
            break;
    }
    return _dt;
}
#pragma endregion

#pragma region CLOCK
//=============================== Clock Functions ===============================

/**
 * @brief Initializes SHIELD RTC Module. 
 */
void Clock::beginClock() {
    while(!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    delay(10);
  }
  Serial.println("\nClock started.");
}

/**
 * @brief Synchronizes device clock with NTP Server.
 */
void Clock::syncClock() {
    Serial.println("Synchronizing date and time with servers...");
    
    _wf.connect("ARGUMIDO", "odimugra023026");
    timeClient.begin();

    int _timer = 0;
    unsigned long _t;
    do {
        _t = _getNTPUnixTime(); //Connects to the NTP Server and gets Unix time
        _timer += 1;
    }while(_timer < 2);

    rtc.adjust(DateTime(year(_t), month(_t), day(_t), hour(_t), minute(_t), second(_t)));
    Serial.println("Clock successfully synchronized!");
}

/**
 * @brief Connects to the defined NTP Server and retrieves the Unix time.
 */
unsigned long Clock::_getNTPUnixTime() {    
    timeClient.update();
    return timeClient.getEpochTime();
}
//===============================================================================
#pragma endregion

#pragma region Wi-Fi
//=============================== Wi-Fi Functions ===============================

/**
 * @brief Connects to a saved Wi-Fi.
 */
void Wifi::connect(char* wifi_ssid, char* wifi_password) {
    this -> _ssid = wifi_ssid;
    this -> _password = wifi_password;

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    
    Serial.print("Connecting to ");
    Serial.print(_ssid);
    Serial.println("...");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected.");
}
//===============================================================================
#pragma endregion