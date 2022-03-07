int buzzerPin = D1;

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, HIGH); // Turn the motor ON
  delay(10000); // Wait for 10 secs
  
  digitalWrite(buzzerPin, LOW); // Turn the motor OFF
  delay(5000); // Wait for 5 secs
}
