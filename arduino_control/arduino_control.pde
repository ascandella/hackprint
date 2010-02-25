int strikePin = 9;
// read off serial port
int incomingByte = 0;

void setup() {
  pinMode(strikePin, OUTPUT);
  // Set up the serial communications
  Serial.begin(9600);
}

void loop() {
  // Read a byte off the serial input when ready
  if ( Serial.available() > 0 ) {
    incomingByte = Serial.read();
    digitalWrite(strikePin, true);
    delay(incomingByte * 1000);
    digitalWrite(strikePin, false);
  }
}
