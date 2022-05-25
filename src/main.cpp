#include <Arduino.h>
#include <ClimateSensor.h>


const int baudrate = 115200;
ClimateSensor climate;


void setup() {
  Serial.begin(baudrate);
  Serial.print("Waiting for climate sensor...");
  while(!climate.begin()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" ");

  climate.setReferenceHeight(223);
  Serial.println("Climate Sensor is ready.\n");
  Serial.println(" ");
}

void loop() { 
  Serial.printf(
    "\rTemperatur: %.2f °C, Feuchtigkeit: %.2f% %, Luftdruck: %.2f hPa, Luftdruck auf Meereshöhe: %.2f Höhe %.2f m", 
    climate.readTemperature(), 
    climate.readHumidity(), 
    climate.readPressure(),
    climate.readSeaLevelPressure(223),
    climate.readAltitude()
  );
  climate.log();
  delay(500);
}