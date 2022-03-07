#include <PCF8574.h>

#define SDAPin D2
#define SCLPin D1

PCF8574 pcf8574(0x20, SDAPin, SCLPin);

void setup() {
  Serial.begin(9600);
  
  pcf8574.pinMode(P0, OUTPUT);

  Serial.print("Init pcf8574...");
  if (pcf8574.begin()){
    Serial.println("OK");
  }else{
    Serial.println("KO");
  }
}

void loop() {
  pcf8574.digitalWrite(P0, HIGH);
  delay(1000);

  pcf8574.digitalWrite(P0, LOW);
  delay(1000);
}
