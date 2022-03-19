#include "SHIELDLib.h"

//Required objects from other third-party libraries
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_ADDRESS, UTC_OFFSET_IN_SECONDS);
File file;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 deviceOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/**
 * @brief Initializes the device type which automatically configures the device based on its type. 
 * 
 * @param _deviceAs Set the device type either as BEACON or as NEURON.
 */
SHIELD::SHIELD() {
    //Do nothing
}

/**
 * @brief Starts the SHIELD device. 
 */
void SHIELD::startDevice() {

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
    //_initSD();          //Initializes the sd card component
    _beginClock();      //Initializes the realtime clock component
    //_loadSystemConfiguration();     //Load System Configuration
    //_syncClock();   //Synchronizes RTC with NTP
    _displayDateTime();

    //_save(AUDIT_DATA, "THIS IS A SAMPLE AUDIT DATA");
    //_save(CIRCADIAN_DATA, "THIS IS A SAMPLE AUDIT DATA");
    //_save(CIRRUS_DATA, "THIS IS A SAMPLE AUDIT DATA");
    //_save(TRANSCRIPT_DATA, "THIS IS A SAMPLE AUDIT DATA");
    //_save(DUMP_DATA, "THIS IS A SAMPLE AUDIT DATA");
    //_save(CONFIG_DATA, "THIS IS A SAMPLE AUDIT DATA");

    Serial.println("Done");
}

/**
 * @brief Opens the OLED Display
 */
void SHIELD::_intitOLEDDisplay() {
    if(!deviceOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("OLED Display failed to start."));
        for(;;);
    }
}

/**
 * @brief Initializes the SD Card Module.
 */
void SHIELD::_initSD() {
    pinMode(pin_csSD, OUTPUT);

    while(!SD.begin(pin_csSD)) {
        Serial.println("Unable to detect the SD Card module!");
        delay(1000);
    }
}

/**
 * @brief Initializes SHIELD RTC Module. 
 */
void SHIELD::_beginClock() {
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
void SHIELD::_loadSystemConfiguration() {
}

/**
 * @brief Synchronizes device clock with NTP Server.
 */
void SHIELD::_syncClock() {
    Serial.println("Synchronizing date and time with servers...");
    
    //_wf.connect("ARGUMIDO", "odimugra023026");
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

String SHIELD::_getFilename(FileToSave _SHIELDFile) {
    String _dest = "";

    switch(_SHIELDFile) {
        case 0: //Audit Folder
            _dest = dir_audit + _slash + "audit_" + getDeviceTime(inUnix) + file_extension;
            break;
        
        case 1: //Circadian Folder
            _dest = dir_circadian + _slash + "circadian_" + getDeviceTime(inUnix) + file_extension;
            break;

        case 2: //Cirrus Folder
            _dest = dir_cirrus + _slash + "cirrus" + file_extension;
            break;

        case 3: //Transcript Folder
            _dest = dir_memories + _slash + "transcript_" + getDeviceTime(inUnix) + file_extension;
            break;

        case 4: //Dump Folder
            _dest = dir_dumps + _slash + "dumps_" + getDeviceTime(inUnix) + file_extension;
            break;

        case 5: //Config Folder
            _dest = dir_core + _slash + "beaconconfig" + file_extension;
            break;
    }

    return _dest;
}

void SHIELD::_save(FileToSave _destinationFile, String _rawdata) {
    String __destination = _getFilename(_destinationFile);    

    do {
        file = SD.open(__destination, FILE_WRITE);
        if(file) {
            Serial.println("Unable to create the file!");
            delay(500);
        }
    }while(!file);

    file.print(_rawdata);
    file.close();

    Serial.println("Done writing.");
}

/**
 * @brief Connects to the defined NTP Server and retrieves the Unix time.
 */
unsigned long SHIELD::_getNTPUnixTime() {    
    timeClient.update();
    return timeClient.getEpochTime();
}

/**
 * @brief Returns device timestamp based on the specifie format.
 * 
 * @param _dtFormat 
 * @return char* 
 */
char* SHIELD::getDeviceTime(DeviceTimeFormat _dtFormat) {
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
 * @brief Connects to a saved Wi-Fi.
 *
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
*/
void SHIELD::_displayDateTime() {
    DateTime now = rtc.now();

    String period = "";
    int h = 0;

    // Converts 24H to 12H with AM/PM designation
    if(now.hour() > 12) {
        h = now.hour() % 12;
        period = " PM";
    }else
    {
        h = now.hour();
        period = " AM";
    }

    // Adding the '0' Padding to minute if minute is lesser than 10
    String Min = (now.minute() < 10) ? "0" + (String)now.minute() : (String)now.minute();
    
    String Date =  (String)now.month() + '/' + (String)now.day() + '/' + now.year();
    String Time = (String)h + ':' + Min + period;

    // Clear the buffer.
    deviceOLED.clearDisplay();
    
    // Display Text
    deviceOLED.setTextSize(2);
    deviceOLED.setTextColor(WHITE);
    deviceOLED.setCursor(0,0);
    deviceOLED.println(Time);
    deviceOLED.println(Date);
    deviceOLED.display();
    
    delay(3000);

    // Clear the buffer.
    deviceOLED.clearDisplay();
    
    // Display Text
    deviceOLED.setTextSize(2);
    deviceOLED.setTextColor(WHITE);
    deviceOLED.setCursor(11,8);
    //display.print("NORMAL");
    deviceOLED.print("SUSPECTED");
    deviceOLED.display();
    //delay(5000);
}
