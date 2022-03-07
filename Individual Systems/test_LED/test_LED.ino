#include <FastLED.h>

#define NUM_LEDS 144
#define DATA_PIN D2

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}
int i = 1;

void loop() {
  //Suspected - RED
  leds[2] = CRGB(0, 255, 0);
  FastLED.show();
  delay(500);
  
  //Healthy - GREEN
  leds[2] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);  

  //Suspected - BLUE
  leds[2] = CRGB(0, 0, 255);
  FastLED.show();
  delay(500);

  //Suspected - GOLD
  leds[2] = CRGB(255,215,0);
  FastLED.show();
  delay(500);

  //Suspected - PURPLE
  leds[2] = CRGB(128,0,128);
  FastLED.show();
  delay(500);

  //Suspected - PINK
  leds[2] = CRGB(255,20,147);
  FastLED.show();
  delay(500);

  /*
  for(i = 1; i < 101; i++) {
     FastLED.setBrightness(i);
    //Positive - RED
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
    delay(50);
  }
  */
}
