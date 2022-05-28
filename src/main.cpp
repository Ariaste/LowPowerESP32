#include <Arduino.h>
#include <Climate.h>
#include <DeepSleep.h>


const char* line = "\n==========================================";
const int mode = 34;
const int baudrate = 115200;
RTC_DATA_ATTR boolean rtcSet = false;
RTC_DATA_ATTR int bootCount = 0;

//Contains sensors and data logger
ClimateSensor climate;

void printAndLogClimate() {
  Serial.printf(
    "\rTemperatur: %.2f °C, Feuchtigkeit: %.2f% %, Luftdruck: %.2f hPa, Luftdruck auf Meereshöhe: %.2f Höhe %.2f m", 
    climate.readTemperature(), 
    climate.readHumidity(), 
    climate.readPressure(),
    climate.readSeaLevelPressure(223),
    climate.readAltitude()
    );
    climate.log();
}

void setup() {
  Serial.begin(baudrate);
  //Print the number of reboots and boolean if RTC is set.
  Serial.printf("%s\nNumber of reboots: %d, RTC set: %s\n", line, bootCount++, rtcSet ? "true" : "false");
  
  //if RTC is not set, enable WiFi and connect to NTP service.
  if(!rtcSet) {
    RTC::set("SSID", "PASSWORD");
    rtcSet = true;
  }

  //toggle switch: high activates deepsleep mode
  pinMode(mode, INPUT);

  
  Serial.print("\nWaiting for climate sensor...");
  climate.begin(rtcSet);
  climate.setReferenceHeight(223);
  Serial.println("Climate Sensor is ready.\n");

  //code for deepsleep mode
  if(digitalRead(mode)) {
    printAndLogClimate();
    deepSleepForSeconds(10);
  }
}

//loop unreachable in deep sleep mode, only runs when deepsleep disabled
void loop() { 
  if(digitalRead(mode)) {
    ESP.restart();
  }
  printAndLogClimate();
  delay(10000);
}