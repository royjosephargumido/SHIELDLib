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
  pinMode(button, INPUT);

  Serial.print("Init pcf8574...");
  if (pcf8574.begin()){
    Serial.println("OK");
  }else{
    Serial.println("KO");
  }

  temp = 0;
}

void loop() {
  temp = digitalRead(button);
  
  Serial.println(temp); 
  if (temp == HIGH) {
    pcf8574.digitalWrite(led, HIGH);
    Serial.println("LED Turned ON");
    delay(1000);
  }
  else {
    pcf8574.digitalWrite(led, LOW);
    Serial.println("LED Turned OFF");
    delay(1000);
  }
}
