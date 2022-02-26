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

Device device(BEACON);

void setup() {
    Serial.begin(115200);

    device.startDevice();
}

void loop() {
}