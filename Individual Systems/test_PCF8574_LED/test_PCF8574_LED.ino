#include <PCF8574.h>
#include <FastLED.h>

#define SDAPin D2
#define SCLPin D1

#define NUM_LEDS 1
#define DATA_PIN D0

int i = 1;

CRGB leds[NUM_LEDS];

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
  pcf8574.digitalWrite(P0, HIGH);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  //Healthy - GREEN
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);

  //Suspected - BLUE
  leds[0] = CRGB(0, 0, 255);
  FastLED.show();
  delay(500);

  //Positive - RED
  leds[0] = CRGB(0, 255, 0);
  FastLED.show();
  delay(500);
  /*
  for(i = 1; i < 101; i++) {
     FastLED.setBrightness(i);
    //Positive - RED
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
    delay(50);
  }*/
}
