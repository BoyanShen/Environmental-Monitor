#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(9, DHT11);

const int chipSelect = 10;

void setup() {
  pinMode(7, INPUT);    //Moisture sensor as input
  pinMode(4, OUTPUT);   //LED as output
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  while (!Serial); // wait for Serial Monitor to open
  Serial.print("Initializing SD card...");
  lcd.setCursor(0,0);
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    lcd.setCursor(0,0);
    lcd.print("Card failed, or");
    lcd.setCursor(0,1);
    lcd.print("not present");
    return;
  }
  Serial.println("card initialized.");
  
  dht.begin();

}

void loop() { //Start of Loop
  delay(5000);        // 5 seconds between samples
  lcd.setCursor(0,0); //reset LCD
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

  //Writing into DATALOG.TXT
  if (dataFile) {
    Serial.println("Temp: " + temperatureString);
    Serial.println("Humi: " + humidityString);
    Serial.println("Brightness:" + brightnessString);

//lcd only prints result when SD card is present
    lcd.print("T:"); //Temperature
    lcd.print(t);
    lcd.print(" H:");//Humidity
    lcd.print(h);
    lcd.setCursor (0,1); //Go to the next line

    if (value<40)
    digitalWrite(4, HIGH);
    else
    digitalWrite(4, LOW);
    lcd.print("B:");//Brightness
    lcd.print(value);

    if (digitalRead(7) == HIGH){
      moistureString = "1";
    }else{
      moistureString = "0";
    }
    dataFile.println("Temperature: " + temperatureString + " Humidity: " + humidityString + " Brightness:" + brightnessString +" Moisture:" + moistureString);
    Serial.println("Moisture:" + moistureString);
    lcd.print(" M:");
    lcd.print(moistureString);
    lcd.print("           ");
    Serial.println(" ");
  }
  dataFile.close(); //save the data at the end of one loop
}
