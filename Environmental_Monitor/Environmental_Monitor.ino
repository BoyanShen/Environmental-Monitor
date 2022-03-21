#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

#define DEBUG true

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(9, DHT11);

const int chipSelect = 10;
String apiKey = "4DJN7QW9K2ZZR0YS";

SoftwareSerial ESP01(2, 3); // RX, TX

void setup() {
  pinMode(7, INPUT);    //Moisture sensor as input
  pinMode(4, OUTPUT);   //LED as output
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  while (!Serial); // wait for Serial Monitor to open

  //Setting up SD Card
  Serial.print(F("Initializing SD card..."));
  lcd.setCursor(0, 0);
  lcd.print(F("Initializing"));

  ESP01.begin(9600);
  //Setup wifi module
  Serial.println();
  sendData(F("AT+RST\r\n"),2000,DEBUG);
  sendData(F("AT+CWMODE=1\r\n"),2000,DEBUG);
  sendData(F("AT+CWJAP=\"iPhone\",\"konopowa\"\r\n"),20000,DEBUG);  
  sendData(F("AT+CIPMUX=0\r\n"),4000,DEBUG);

  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    lcd.setCursor(0, 0);
    lcd.print(F("Card failed, or"));
    lcd.setCursor(0, 1);
    lcd.print(F("not present"));
    return;
  }
  Serial.println(F("card initialized."));
  dht.begin();
}

void loop() { //Start of Loop
  delay(1000);        // 5 seconds between samples
  lcd.setCursor(0, 0); //reset LCD

  File dataFile = SD.open("datalog.txt", FILE_WRITE); // use 8.3 file name

  //Initialize everything

  //Read Temperature & Humidity using DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String humidityString = String(h);
  String temperatureString = String(t);
  String moistureString = "0";

  

  //Read from LDR
  int value = analogRead(A0);
  String brightnessString = String(value);

  Serial.println("Temp: " + temperatureString);
  Serial.println("Humi: " + humidityString);
  Serial.println("Brightness:" + brightnessString);

  lcd.print(F("T:")); //Temperature
  lcd.print(t);
  lcd.print(F(" H:"));//Humidity
  lcd.print(h);
  lcd.print(F("             "));
  lcd.setCursor (0, 1); //Go to the next line

  //LDR
  if (value < 40)
    digitalWrite(4, HIGH);
  else
    digitalWrite(4, LOW);
  lcd.print(F("B:"));//Brightness
  lcd.print(value);

  //Moisture Sensor
  if (digitalRead(7) == HIGH)
    moistureString = "1";
  else
    moistureString = "0";

  Serial.println("Moisture:" + moistureString);
  lcd.print(F(" M:"));
  lcd.print(moistureString);
  lcd.print(F("           "));
  Serial.println(F(" "));


  //Writing into DATALOG.TXT
  if (dataFile)
    dataFile.println("Temperature: " + temperatureString + " Humidity: " + humidityString + " Brightness:" + brightnessString + " Moisture:" + moistureString);
  dataFile.close(); //save the data at the end of one loop


  // Make TCP connection
  String cmd = F("AT+CIPSTART=\"TCP\",\"");
  cmd += F("184.106.153.149"); // Thingspeak.com's IP address  
  cmd += F("\",80\r\n");
  sendData(cmd,4000,DEBUG);

  // Prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += temperatureString;
  getStr +="&field2=";
  getStr += humidityString;
  getStr +="&field3=";
  getStr += brightnessString;
  getStr +="&field4=";
  getStr += moistureString;
  getStr += "\r\n";

  // Send data length & GET string
  ESP01.print(F("AT+CIPSEND="));
  ESP01.println (getStr.length());
  //Serial.print(F("AT+CIPSEND="));
  //Serial.println (getStr.length());  
  delay(500);
  if( ESP01.find( ">" ) )
  {
    //Serial.print(F(">"));
    sendData(getStr,4000,DEBUG);
  }

  // Close connection, wait a while before repeating...
  sendData(F("AT+CIPCLOSE"),18000,DEBUG); // thingspeak needs 15 sec delay between updates
  sendData(F("AT+RST\r\n"),2000,DEBUG);   // good habit to reset after every loop
}

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  ESP01.print(command);
  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (ESP01.available())
    {
      // "Construct" response from ESP01 as follows
      // - this is to be displayed on Serial Monitor.
      char c = ESP01.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    //Serial.print(response);
  }

  return (response);
}
