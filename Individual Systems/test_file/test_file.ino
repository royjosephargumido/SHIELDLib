#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define pin_csSD D8         //SD Card Chip Select Pin in NodeMCU
#define slash '/'

#define folder_Data "Data"
#define folder_Audit "Audit"
#define folder_Circadian "Circadian"
#define folder_CIRRUS "CIRRUS"
#define folder_Memories "Memories"

#define folder_System "System"
#define folder_Docu "Documentation"
#define folder_Core "Core"

#define fn_CoreConfiguration "BeaconConfig.shield"

const String dir_audit = String() + folder_Data + slash + folder_Audit + slash;
const String dir_circadian = String() + folder_Data + slash + folder_Circadian + slash;
const String dir_cirrus = String() + folder_Data + slash + folder_CIRRUS + slash;
const String dir_memories = String() + folder_Data + slash + folder_Memories + slash;
const String dir_core = String() + folder_System + slash + folder_Core + slash + fn_CoreConfiguration;

File file;
StaticJsonDocument<192> rawdata_config;

enum FileToSave {
  CONFIG_DATA,
  
}

void setup() {
}

void loop() {
}
