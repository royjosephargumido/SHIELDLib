#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//====================================================
DateTime now;     // Creates a DateTime object
RTC_DS3231 rtc;   // Creates the RTC object

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void beginClock() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    
    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void displayOLED(String _lineone, String _linetwo) {
  // Clear the buffer.
  display.clearDisplay();
  
  // Display Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(_lineone);
  display.println(_linetwo);
  display.display();
}

void getDateTime() {
  DateTime now = rtc.now();
  
  String HF = "";
  int h = 0;
  String m = "";
  String s = "";

  // COnverting 24H to 12H with AM/PM designation
  if(now.hour() > 12) {
    h = now.hour() % 12;
    HF = " PM";
  }else
  {
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
  
  Serial.print(Date);
  Serial.print(' ');
  Serial.print(Time);
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

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("System running.");
  delay(2000);

  beginClock();
  
  //display.startscrollright(0x00, 0x0F);
}

void loop() {
  //getDateTime();

  DateTime now = rtc.now();

  String period = "";
  int h = 0;

  // Converts 24H to 12H with AM/PM designation
  if(now.hour() > 12) {
    h = now.hour() % 12;
    period = " PM";
  }else
  {
    h = now.hour();
    period = " AM";
  }

  // Adding the '0' Padding to minute if minute is lesser than 10
  String Min = (now.minute() < 10) ? "0" + (String)now.minute() : (String)now.minute();
  
  String Date =  (String)now.month() + '/' + (String)now.day() + '/' + now.year();
  String Time = (String)h + ':' + Min + period;

  // Clear the buffer.
  display.clearDisplay();
  
  // Display Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(Time);
  display.println(Date);
  display.display();
  
  delay(3000);

  // Clear the buffer.
  display.clearDisplay();
  
  // Display Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(11,8);
  //display.print("NORMAL");
  display.print("SUSPECTED");
  //display.startscrollright(0x00, 0x0F);
  display.display();
  delay(5000);
  //display.stopscroll();
}
