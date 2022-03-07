int touchPin = D2;// connect output from TTP223 to this
int val = 0; 
int relayPin = D1;// Connected to relay

bool led_on = false;

void setup() {
  Serial.begin(9600);
  pinMode(touchPin, INPUT); 
  pinMode(relayPin, OUTPUT);
}

void loop() {  
  val = digitalRead(touchPin);
    
  if(val == 1){
    Serial.println("Touched");
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  //delay(100);
}
