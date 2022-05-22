#include <Arduino.h>
#include <Adafruit_HTU21DF.h>
#include <RGB.h> 
#include <DeepSleep.h>
#include <SDCard.h>


const int baudrate = 115200;
Adafruit_HTU21DF climate;
RGB led(25, 26, 27);
SDCard sd;

void setup() {
  led.setColor(0, 0, 255);
  Serial.begin(baudrate);
  while(!climate.begin()) {
    led.setColor(255, 255, 0);
    Serial.print(".");
    delay(500);
  }
  led.setColor(0, 255, 0);
  Serial.println("\nHTU21DF is ready.\n");

  sd.begin();
  sd.listDir();
}

void loop() { 
  //Serial.printf("\rTemperatur: %.2f°C, Feuchtigkeit: %.2f%%", climate.readTemperature(), climate.readHumidity());
  //delay(500);
}