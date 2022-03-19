/**
 * @file SHIELDDependents.h
 * @author Roy Joseph Argumido (royjosephargumido@outlook.com)
 * @brief SHIELD's Standard Defaults Library
 * @version 0.1
 * @date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 * LICENSE: GNU Affero General Public License v3.0
 */

#pragma once
#define SHIELDDEPENDENTS_H

//PIN CONFIGURATION
#define pin_csSD                D8     //SD Card Chip Select Pin
//Network Time Protocol
#define UTC_OFFSET_IN_SECONDS   28800                 //Formats the time in GMT+08:00 Manila, Philippines
#define NTP_SERVER_ADDRESS      "asia.pool.ntp.org"      //Network Time Protocol (NTP) Server
//File System
#define slash                   '/'
#define file_extension          ".shield"
#define folder_Data             "Data"
#define folder_Audit            "Audit"
#define folder_Circadian        "Circadian"
#define folder_CIRRUS           "CIRRUS"
#define folder_Memories         "Memories"
#define folder_System           "System"
#define folder_Core             "Core"
#define folder_Docu             "Documentation"
#define folder_Dumps            "Dumps"
#define fn_CoreConfiguration    "BeaconConfig"
//Baud
#define baud_rate           115200
//OLED
#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT       32  // OLED display height, in pixels
//Tag
#define TAG_VALIDITY        96  //Validity of Tags in days
#define KEY_VALIDITY        15  //Validity of Keys in minutes
#define PROFILE_VALIDITY    15  //Validity of Profile in minutes
//Close Contact Classification Period (C3P)
#define C3P_BEGIN           2   //Start of Transcript Retrieval
#define C3P_END             14  //End of Transcript Retrieval

const String dir_audit = String() + folder_Data + slash + folder_Audit;
const String dir_circadian = String() + folder_Data + slash + folder_Circadian;
const String dir_cirrus = String() + folder_Data + slash + folder_CIRRUS;
const String dir_dumps = String() + folder_Data + slash + folder_Dumps;
const String dir_memories = String() + folder_Data + slash + folder_Memories;
const String dir_core = String() + folder_System + slash + folder_Core;

/**
 * @brief Types of SHIELD files 
 */
enum FileToSave {
    AUDIT_DATA,
    CIRCADIAN_DATA,
    CIRRUS_DATA,
    TRANSCRIPT_DATA,
    DUMP_DATA,
    CONFIG_DATA,
    PROFILE_DATA
};

enum ErrorCodes {
   INIT_SD_FAILED,
   INIT_RTC_FAILED,
   INIT_OLED_FAILED
};

/**
 * @brief SHIELD Timestamp Formats
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
    U3, //Deactivated
};