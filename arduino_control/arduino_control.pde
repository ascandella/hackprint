int ledPin = 9;
// read off serial port
int incomingByte = 0;

void setup() {
  // Set up the serial communications
  Serial.begin(9600);
}

void loop() {
  
  if ( Serial.available() > 0 ) {
    incomingByte = Serial.read();
    analogWrite(ledPin, 200);
    delay(incomingByte * 1000);
    analogWrite(ledPin, 0);
  }
}
