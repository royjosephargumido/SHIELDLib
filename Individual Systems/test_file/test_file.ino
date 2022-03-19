#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define pin_csSD D8     //SD Card Chip Select Pin

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

const String dir_audit = String() + folder_Data + slash + folder_Audit;
const String dir_circadian = String() + folder_Data + slash + folder_Circadian;
const String dir_cirrus = String() + folder_Data + slash + folder_CIRRUS;
const String dir_dumps = String() + folder_Data + slash + folder_Dumps;
const String dir_memories = String() + folder_Data + slash + folder_Memories;
const String dir_core = String() + folder_System + slash + folder_Core;
#pragma endregion

File file;

enum DataType {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA
};

enum DeviceTimeFormat {
    inUnix,
    inHumanReadableFormat
};



void _initSD() {
    pinMode(pin_csSD, OUTPUT);

    while(!SD.begin(pin_csSD)) {
        Serial.println("Unable to detect the SD Card module!");
        delay(1000);
    }
}

void _initDirectories() {
    //Data Folder
    SD.mkdir(dir_audit);
    SD.mkdir(dir_circadian);
    SD.mkdir(dir_cirrus);
    SD.mkdir(dir_memories);

    //System Folder
    SD.mkdir(dir_dumps);
    SD.mkdir(dir_core);
}

unsigned long __getDeviceTime(DeviceTimeFormat _dtFormat) {
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

void _save(FileToSave _destinationFile, String _rawdata) {
    String __destination = "";

    switch(_destinationFile) {
        case 0: //Audit Folder
            __destination = dir_audit + slash + "audit_" + __getDeviceTime(inUnix).toString() + file_extension;
            break;
        
        case 1: //Circadian Folder
            break;

        case 2: //Cirrus Folder
            break;

        case 3: //Transcript Folder
            break;

        case 4: //Dump Folder
            break;

        case 5: //Config Folder
            break;
    }

    if(!(__destination == "")) {
        do {
            file = SD.open(__destination, FILE_WRITE);
            if(file) {
                Serial.println("Unable to create the file!");
                delay(500);
            }
        }while(!file);
    }    
}

void setup() {
  Serial.begin(9600);

  _initSD();
  _initDirectories();
}

void loop() {
}
