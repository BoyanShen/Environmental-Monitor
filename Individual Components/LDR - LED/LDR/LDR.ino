// LDR sample code

void setup() {
  pinMode(4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
 int value = analogRead(A0);

 Serial.println(value);

 if (value<20)
  digitalWrite(4, HIGH);

 else
  digitalWrite(4, LOW);

 delay(100);
  /*
  if (analogRead(A0) > 600){ // may need to adjust threshold
    // it is bright, do the necessary...
  analogWrite(11, 250);
  delay(1000);
  analogWrite(11, 150);
  delay(1000);
  analogWrite(11, 50);
  delay(1000);  
  }
  else{
  analogWrite(11, 50);
  delay(1000);
  analogWrite(11, 150);
  delay(1000); 
  analogWrite(11, 250);
  delay(1000);
  }
  delay(100);
  */
}
