#include <FastLED.h>

#define NUM_LEDS 5
#define DATA_PIN D2

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}
int i = 1;
int counter = 1;

void loop() {
  for(i = 0; i < NUM_LEDS; i++) {
    switch(counter) {
      case 1:
        //Suspected - RED
        leds[i] = CRGB(0, 255, 0);
        FastLED.show();
        break;

      case 2:
        //Healthy - GREEN
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
        break;

      case 3:
        //Healthy - BLUE
        leds[i] = CRGB(0, 0, 255);
        FastLED.show();
        break;
        
      case 4:
        //Healthy - PURPLE
        leds[i] = CRGB(128,0,128);
        FastLED.show();
        break;

      case 5:
        //Healthy - GOLD
        leds[i] = CRGB(255,215,0);
        FastLED.show();
        break;
    }
    if(i == 4) {
      counter++;
      delay(1000);
    }
  }
  

    if(counter == 5) {
      counter = 0;
    }
}
