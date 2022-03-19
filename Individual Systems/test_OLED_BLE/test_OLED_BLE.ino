#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SoftwareSerial ble(D3, D4); // RX, TX

void setup() {
  Serial.begin(9600);
  ble.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void sendData() {
  Serial.println("Sending Bluetooth Message...");
  ble.println("Data recieved!");
}


String readSerial(){
  char reply[200];
  int i = 0;

  while (ble.available()) {
    reply[i] = ble.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';

  Serial.print(reply);
  return String(reply);
}

void loop() {
  String _data = readSerial();
  
  /*
   * if(!(_data.length() == 0)) {
    // Clear the buffer.
    display.clearDisplay();
    
    // Display Text
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Data");
    display.println("received!");
    display.display();
    //delay(5000);
  }else {
    display.clearDisplay();
    display.display();
    //delay(5000);
  }
   */
}
