/**
 * @file test.ino
 * @author Roy Joseph B. Argumido (royjosephargumido@outlook.com)
 * @brief Main Arduino SHIELD Sketch
 * @version 0.1
 * @date 2022-02-10
 * 
 * @copyright Copyright (c) 2022 Team SHIELD
 * 
 */

#include "SHIELDLib.h"

SHIELDDevice beacon;

int profile_valid_until = PROFILE_VALIDITY;     //Generation of Profile in minutes, using default
uint32_t start_contact_trace = C3P_BEGIN;       //Start of Transcript Retrieval
uint32_t end_contact_trace = C3P_END;           //End of Transcript Retrieval

void setup() {
    beacon.startDevice();
}

void loop() {
}