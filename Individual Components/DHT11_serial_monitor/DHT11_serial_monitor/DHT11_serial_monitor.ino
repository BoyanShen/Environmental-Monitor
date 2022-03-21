// DHT11 (temperature humidity sensor) sample code

#include "DHT.h"
DHT dht(9, DHT11); // DHT22 for DHT22/AM2302/AM2321, or DHT21 for DHT21/AM2301
    // data pin connected to pin 2

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // 2 seconds between samples
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
}
