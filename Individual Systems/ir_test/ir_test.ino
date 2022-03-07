#include <IRremote.hpp>

#define IR_RECEIVE_PIN D2

void setup() {
  Serial.begin(115200);
  Serial.println("IR started.");
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  IrSender.begin();
}

void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      IrReceiver.printIRResultShort(&Serial); // optional use new print version
      IrReceiver.resume(); // Enable receiving of the next value
  }
}
