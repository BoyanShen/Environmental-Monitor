// moisture sensor sample code

void setup() {
  pinMode(7, INPUT);
  // assume sensor output connected to pin 1
}

void loop() {
  if (digitalRead(7) == HIGH){
    digitalWrite(4, HIGH);
    // moisture detected, do the necessary...
  }
  delay(100);
}
