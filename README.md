# LowPowerESP32
The code is developed using VS Code and platform.io.

## Used Hardware:
- ESP32 
- SD Card Modul (communication over SPI)
- HTU21DF temperature and humidity sensor (communication over I2C)
- BMP180 temperature and barometric pressure sensor (communication over I2C)
- toggle switch

## The project contains a file Climate.h which contains the following classes:
- ```ClimateTimeStamp``` for creating time stamps
- ```ClimateDataLogger``` for logging sensor data to an SD card
- ```ClimateSensor``` which uses the two classes above and encapsulates the HTU21DF and BMP180
- ```RTC``` a static class for initialisation of the internal real time clock

## Active Mode vs Deep Sleep Mode
The main.cpp provides two mode chosen by a toogle switch: Active Mode and Deep Sleep Mode. 
In Active Mode, the RTC is initalised over WiFi and a loop begins which measures and logs the climate all 10 seconds. 
Im Deep Sleep Mode, the RTC is initialised once after the first startup and is hold in the sleep period. After setting up the components 
one measurement is made and the ESP32 goes into deep sleep. It wakes up 10 seconds later.
