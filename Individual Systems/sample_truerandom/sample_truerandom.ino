#include <ESP8266TrueRandom.h>

void setup() {
  Serial.begin(115200);

  Serial.print("I threw a random die and got ");
  Serial.print(random(1,7));

  Serial.print(". Then I threw a TrueRandom die and got ");
  Serial.println(ESP8266TrueRandom.random(1,7));

}

void loop() {
  ; // Do nothing
}
