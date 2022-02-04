#include "SHIELDLib.h"

Device device(BEACON);

void setup() {
    Serial.begin(115200);

    device.startDevice();
}

void loop() {
}