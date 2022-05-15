#include "SHIELDLib.h"

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    shield.startDevice();       //Initializes and begins the SHIELD
    Serial.println("Device running.");
}

void loop() {
    shield.startSHIELD();
    shield.handleWebServer();
}