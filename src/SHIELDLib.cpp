#include "SHIELDLib.h"

//Required objects from other third-party libraries
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);
File file;
Wifi _wf;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display(String _message) {
    switch(currentDisplayChannel) {
        case useBothDisplays:   /*SYSTEM DEFAULT DISPLAY*/
            Serial.print(_message);
            //Display message in OLED
            break;

        case useSerialMonitor:
            Serial.print(_message);
            break;
        
        case useOLEDDisplay:
            //Display message in OLED only
            break;
    }
}

/**
 * @brief Initializes the device type which automatically configures the device based on its type. 
 * 
 * @param _deviceAs Set the device type either as BEACON or as NEURON.
 */
SHIELDDevice::SHIELDDevice() {
    //Do nothing
}

/**
 * @brief Starts the SHIELD device. 
 */
void SHIELDDevice::startDevice() {

    while(!Serial){
        /* 
         * Checks if Serial port is open,
         * otherwise, the device opens the default serial port.
         * See SHIELDLib.h baud_rate property
        */ 
        Serial.begin(baud_rate);    //Enables Serial baud
    }

    //Initializes all components
    _intitOLEDDisplay(); //Opens the OLED Display
    _initSD();          //Initializes the sd card component
    _beginClock();      //Initializes the realtime clock component
    _loadSystemConfiguration();     //Load System Configuration
    _syncClock();   //Synchronizes RTC with NTP
    FastLED.addLeds<WS2811, pin_dataLED, RGB>(leds, 1);     //Enables the RGB Led
}

/**
 * @brief Opens the OLED Display
 */
void SHIELDDevice::_intitOLEDDisplay() {
    if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

/**
 * @brief Initializes the SD Card Module.
 */
void SHIELDDevice::_initSD() {
    pinMode(pin_csSD, OUTPUT);

    while(!SD.begin(pin_csSD)) {
        Serial.println("Unable to detect the SD Card module!");
        delay(1000);
    }
}

/**
 * @brief Initializes SHIELD RTC Module. 
 */
void SHIELDDevice::_beginClock() {
    while(!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    delay(10);
  }
  Serial.println("\nClock started.");
}

/**
 * @brief Load system configuration from SD Card.
 */
void SHIELDDevice::_loadSystemConfiguration() {
    char config_data[2000];
    int index = 0;

    file.open(dir_core);

    while(!file) {
        Serial.println("Unable to load settings!");
        delay(1000);
        file = SD.open(dir_core);
    }

    while(file.available()) {
        config_data[index] = file.read();
        index++;
    }
    data[i] = '\0';

    //Decrypt raw data
    //Deserialize JSON data

    Serial.println("Settings successfully loaded into the cache.");
    file.close();
}

/**
 * @brief Synchronizes device clock with NTP Server.
 */
void SHIELDDevice::_syncClock {
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
unsigned long SHIELDDevice::_getNTPUnixTime() {    
    timeClient.update();
    return timeClient.getEpochTime();
}

/**
 * @brief Set the display channel.
 * 
 * @param _dchannel 
 */
void SHIELDDevice::setDisplayChannel(DisplayChannel _dchannel) {
    currentDisplayChannel = _dchannel;
}

/**
 * @brief Returns device timestamp based on the specifie format.
 * 
 * @param _dtFormat 
 * @return char* 
 */
char* SHIELDDevice::getDeviceTime(DeviceTimeFormat _dtFormat) {
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

/**
 * @brief This lights up the builtin LED 
 */
void SHIELDDevice::lightupLED() {
    int _status = 1;    //Get the HealthStatus of the device
 
    switch(_status) {
        case 1:     //U0 - Healthy or Normal            
            leds[0] = CRGB(0, 255, 0);  //Green
            break;

        case 2:     //U1 - Suspected
            leds[0] = CRGB(255, 165, 0);  //Orange
            break;

        case 3:     //U2 - Positive
            leds[0] = CRGB(255, 0, 0);  //Red
            break;

        case 4:     //U3 - Recovered
            leds[0] = CRGB(0, 0, 255);  //Blue
            break;

        case 5:     //U5 and U6 - System Flag
            leds[0] = CRGB(128,0,128);  //Purple
            break;
    }

    FastLED.show();
}

/**
 * @brief This generates the tag.
 * 
 * @return char* 
 */
char* SHIELDDevice::generateTag() {
    /*
        SHIELD Protocol

        1. Preprocessing Block: Converts raw data to JSON
        2. Encryption: Encrypts JSON data
        3. Error-Correction: Adds error correction to the payload
    */

    //Performs the protocol PREPROCESSING BLOCK
    /*
    StaticJsonDocument<384> doc;
    char* _rawpayload;

    doc["HS"] = "U3";
    doc["CV"] = 1351824120;
    doc["MA"] = "506583791D47";
    doc["CN"] = "9971432991";
    doc["CT"] = "1642088234";
    doc["ET"] = "1642089134";
    doc["IV"] = "y$B&E)H@McQfThWmZq4t7w!z%C*F-JaN";
    doc["IK"] = "3s6v9y$B&E)H@McQfTjWnZr4u7w!z%C*";
    */
    serializeJson(doc, _rawpayload);
    //End Preprocessing Block
    return _rawpayload;
}

/**
 * @brief Connects to a saved Wi-Fi.
 */
void SHIELDDevice::_connect(char* wifi_ssid, char* wifi_password) {
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