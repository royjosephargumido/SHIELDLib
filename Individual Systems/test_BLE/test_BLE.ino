#include <SoftwareSerial.h>

SoftwareSerial ble(D3, D4); // RX, TX

void setup() {
  Serial.begin(9600);
  ble.begin(9600);
}

void sendData() {
  Serial.println("Sending Bluetooth Message...");
  ble.println("Data recieved!");
}


String readSerial(){
  char reply[101];
  int i = 0;

  while (ble.available()) {
    reply[i] = ble.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  
  return String(reply);
}

void loop() {
  Serial.print(readSerial());
}
