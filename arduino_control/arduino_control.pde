int ledPin = 9;
// read off serial port
int incomingByte = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  // Set up the serial communications
    Serial.begin(9600);
}

void loop() {
  
  if ( Serial.available() > 0 ) {
    incomingByte = Serial.read();
    digitalWrite(ledPin, true);
    delay(incomingByte * 1000);
    digitalWrite(ledPin, false);
  }
}
