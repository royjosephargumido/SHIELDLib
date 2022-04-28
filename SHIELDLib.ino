#include "SHIELDLib.h"

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    shield.startDevice();       //Initializes and begins the SHIELD
    Serial.println("Device running.");
}

void loop() {
    shield.protocolbegin();     //Begins the SHIELD Protocol and Cryptography Modules
    shield.listen();            //Sends profile and recieves transcript to and from other devices
    shield.getHealthStatus();   //Displays HealthStatus when button pressed, else display date and time
}