// SD card module sample code

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for Serial Monitor to open
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  int sensor = analogRead(A0);
  String dataString = String(sensor);

  File dataFile = SD.open("datalog.txt", FILE_WRITE); // use 8.3 file name
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString); // echo on Serial Monitor
  }
  else {
    Serial.println("error opening datalog.txt");
  }

  delay(5000);
}
