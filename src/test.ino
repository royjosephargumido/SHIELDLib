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

/**
 * @brief Types of SHIELD files 
 * 
 */
enum FileToSave {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA
};

void save(FileToSave _filetosave, String _rawdata) {
    switch(_filetosave) {
        case AUDIT_DATA:
            break;

        case CIRCADIAN_DATA:
            break;

        case CIRRUS_DATA:
            break;

        case TRANSCRIPT_DATA:
            break;

        case DUMP_DATA:
            break;

        case CONFIG_DATA:
            break;
    }
}

void setup() {
}

void loop() {
}
