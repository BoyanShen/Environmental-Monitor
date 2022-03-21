#include <SoftwareSerial.h>

#define DEBUG true

// LED at D13 
int ledPin = 13;

// Potentiometer at A0
int potPin = 0;

// replace with your Thingspeak channel's API key!!!
String apiKey = "1DY7HFILLTT76FUE"; // ****************************************************************** Change this!

// UNO's D10 connected to ESP's TX
// UNO's D11 connected to ESP's RX via resistor network
SoftwareSerial ESP01(10, 11); // RX, TX

void setup() {                
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
  while (!Serial){
  }
  Serial.println("Starting...");
  ESP01.begin(9600);
}

void loop() {
  // Reset ESP8266, put it into mode 1 i.e. STA only, make it join hotspot / AP, 
  // establish single connection
  Serial.println();
  sendData("AT+RST\r\n",2000,DEBUG);
  sendData("AT+CWMODE=1\r\n",2000,DEBUG);
  sendData("AT+CWJAP=\"BigRedAnt\",\"sp470819\"\r\n",4000,DEBUG);  
   // ****************************************************************** Change these!
  sendData("AT+CIPMUX=0\r\n",2000,DEBUG);

  // Blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);

  // Read potentiometer value
  int sensorValue = analogRead(A0); // 10 bit result: 0 - 1023
  float voltage = sensorValue * (5.0 / 1023.0); // 0V - 5V
  String temp = String(voltage); // convert to string  
  Serial.println(temp);

  // Make TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // Thingspeak.com's IP address  
  // ****************************************************************** Change this!
  cmd += "\",80\r\n";
  sendData(cmd,2000,DEBUG);

  // Prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += temp;
  getStr += "\r\n";
  
  // Send data length & GET string
  ESP01.print("AT+CIPSEND=");
  ESP01.println (getStr.length());
  Serial.print("AT+CIPSEND=");
  Serial.println (getStr.length());  
  delay(500);
  if( ESP01.find( ">" ) )
  {
    Serial.print(">");
    sendData(getStr,2000,DEBUG);
  }

  // Close connection, wait a while before repeating...
  sendData("AT+CIPCLOSE",16000,DEBUG); // thingspeak needs 15 sec delay between updates
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    ESP01.print(command);
    long int time = millis();

    while( (time+timeout) > millis())
    {
      while(ESP01.available())
      {
        // "Construct" response from ESP01 as follows 
         // - this is to be displayed on Serial Monitor. 
        char c = ESP01.read(); // read the next character.
        response+=c;
      }  
    }

    if(debug)
    {
      Serial.print(response);
    }
    
    return (response);
}
