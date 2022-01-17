#include "SHIELDLib.h"

Device device(BEACON);

void setup() {
    Serial.begin(115200);

    device.startDevice();

    Serial.println(device.getDeviceType());
    Serial.println(device.getDeviceTime(inUnix));
    Serial.println(device.getDeviceTime(inHumanReadableFormat));

    Serial.println("==================");
}

void loop() {
}