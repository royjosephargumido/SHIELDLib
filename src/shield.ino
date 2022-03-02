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

void setup() {
    beacon.startDevice();
}

void loop() {
}