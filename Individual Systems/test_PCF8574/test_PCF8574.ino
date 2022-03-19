#include <PCF8574.h>

#define SDAPin D2
#define SCLPin D1
#define led P0

const int button = D3;
int temp = 0;

PCF8574 pcf8574(0x20, SDAPin, SCLPin);

void setup() {
  Serial.begin(115200);
  
  pcf8574.pinMode(P0, OUTPUT);

  Serial.print("Init pcf8574...");
  if (pcf8574.begin()){
    Serial.println("OK");
  }else{
    Serial.println("KO");
  }

  temp = 0;
}

void loop() {
  tone(P0, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(P0);     // Stop sound...
  delay(1000);        // ...for 1sec
}
