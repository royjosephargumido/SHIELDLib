/**
 * @file test.ino
 * @author Roy Joseph B. Argumido (royjosephargumido@outlook.com)
 * @brief Main Arduino SHIELD Sketch
 * @version 0.1
 * @date 2022-02-10
 * 
 * @copyright Copyright (c) 2022 Team SHIELD
 * 
 */

#include "SHIELDLib.h"

SHIELDDevice beacon;

void setup() {
    beacon.startDevice();
}

void loop() {
    DateTime now = rtc.now();
  
    String HF = "";
    int h = 0;
    String m = "";
    String s = "";

    // COnverting 24H to 12H with AM/PM designation
    if(now.hour() > 12) {
        h = now.hour() % 12;
        HF = " PM";
    }else {
        h = now.hour();
        HF = " AM";
    }

    // Adding the '0' Padding to minute if minute is lesser than 10
    if(now.minute() < 10) { m = "0" + (String)now.minute(); }
    else { m = (String)now.minute(); }

    // Adding the '0' Padding to second if second is lesser than 10
    if(now.second() < 10) { s = "0" + (String)now.second(); }
    else { s = (String)now.second(); }
    
    String Date =  (String)now.month() + '/' + (String)now.day() + '/' + now.year();
    String Time = (String)h + ':' + (String)m + ':' + (String)s + HF;
    
    Serial.println(Date);
    Serial.println(Time);
    Serial.println();

    // Clear the buffer.
    display.clearDisplay();
    
    // Display Text
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(Date);
    display.println(Time);
    display.display();
    delay(1000);
}