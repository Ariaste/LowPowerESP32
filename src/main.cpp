#include <Arduino.h>
#include <Climate.h>
#include <DeepSleep.h>


const char* line = "\n==========================================";
const int mode = 34;
const int baudrate = 115200;
RTC_DATA_ATTR boolean rtcSet = false;
RTC_DATA_ATTR int bootCount = 0;

//Contains sensors and data logger
ClimateSensor climate("WLAN-277967", "95826639775575742230");
ClimateTimeStamp rtc("WLAN-277967", "95826639775575742230");


void setup() {
  Serial.begin(baudrate);
  Serial.printf("%s\nNumber of reboots: %d, RTC set: %s\n", line, bootCount++, rtcSet ? "true" : "false");
  
  if(!rtcSet) {
    rtc.setRealTimeClock();
    rtcSet = true;
  }

  pinMode(mode, INPUT);

  
  Serial.print("\nWaiting for climate sensor...");
  climate.begin(rtcSet);
  climate.setReferenceHeight(223);
  Serial.println("Climate Sensor is ready.\n");

  if(digitalRead(mode)) {
    Serial.printf(
    "\rTemperatur: %.2f °C, Feuchtigkeit: %.2f% %, Luftdruck: %.2f hPa, Luftdruck auf Meereshöhe: %.2f Höhe %.2f m", 
    climate.readTemperature(), 
    climate.readHumidity(), 
    climate.readPressure(),
    climate.readSeaLevelPressure(223),
    climate.readAltitude()
    );
    climate.log();
    deepSleepForSeconds(10);
  }
}

//unreachable in deep sleep mode
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
  delay(10000);
}