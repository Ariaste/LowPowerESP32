#include <Arduino.h>
#include <Adafruit_HTU21DF.h>
#include <RGB.h> 
#include <DeepSleep.h>
#include <SDCard.h>
#include <ClimateSensor.h>


const int baudrate = 115200;
ClimateSensor climate;
RGB led(25, 26, 27);
SDCard sd;

void setup() {
  led.setColor(255, 0, 0);
  Serial.begin(baudrate);
  Serial.print("Waiting for climate sensor...");
  while(!climate.begin()) {
    led.setColor(255, 255, 0);
    Serial.print(".");
    delay(500);
  }
  led.setColor(255, 255, 0);
  climate.setReferenceHeight(223),
  Serial.println("Climate Sensor is ready.\n");

  sd.begin();
  sd.listDir();
  Serial.println(" ");
  led.setColor(0, 255, 0);
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
  delay(500);
}