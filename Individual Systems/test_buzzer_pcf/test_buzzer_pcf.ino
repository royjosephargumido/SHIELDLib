#include <PCF8574.h>

#define SDAPin D2
#define SCLPin D1

PCF8574 pcf8574(0x20, SDAPin, SCLPin);

//Specify digital pin on the Arduino that the positive lead of piezo buzzer is attached.
int piezoPin = P0;

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

  /*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long teh tone plays
  */
  tone(piezoPin, 1000, 500);
}
