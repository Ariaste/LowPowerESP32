/**
 * @file Climate.h
 * @author Patrick Fock (you@domain.com)
 * @brief This library provides classes for measuring and logging climate data provided by the HTU21DF and the BMP085 or BMP180 sensors.
 * @version 1.0
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cmath>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP085.h>
#include <SDCard.h>
#include <WiFi.h>
#include <time.h>

/**
 * @brief A class for creating timeStamps.
 * 
 */
class ClimateTimeStamp {

    private:
        const char* _ssid;
        const char* _password;
        const char* _ntpServer;
        long _gmtOffset_sec = 0;
        int _daylightOffset_sec = 3600;

    public:

        /**
         * @brief Construct a new ClimateTimeStamp object
         * 
         * @param ssid SSID of your WiFi network
         * @param password password of WiFi network
         * @param ntpServer time server
         * @param gmtOffset_sec time offset from GMT in seconds
         * @param daylightOffset_sec daylightOffset, usually 3600
         */
        ClimateTimeStamp(
            const char* ssid = "SSID", 
            const char* password = "PASSWORD", 
            const char* ntpServer = "ptbtime1.ptb.de", 
            long gmtOffset_sec = 3600, 
            int daylightOffset_sec = 3600) {
                _ssid = ssid;
                _password = password;
                _ntpServer = ntpServer;
                _gmtOffset_sec = gmtOffset_sec;
                _daylightOffset_sec = daylightOffset_sec;
        }

        /**
         * @brief Sets the real time clock;
         * 
         */
        void setRealTimeClock() {
            Serial.begin(115200);
            Serial.print("Setting Real Time Clock (RTC)");
            WiFi.begin(_ssid, _password);
            while(WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(500);
            };
            configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);
            Serial.println("\nRTC initalised");
            getTimeStamp();
            WiFi.mode(WIFI_OFF);
        }

        /**
         * @brief Returns a time stamp as String
         * 
         * @return String time stamp
         */
        String getTimeStamp() {
            struct tm timeInfo;
            if(!getLocalTime(&timeInfo)){
                return "Failed to obtain time";
            }
            String timeString = "";
            timeString.concat(timeInfo.tm_mday);
            timeString.concat(".");
            timeString.concat(timeInfo.tm_mon + 1);
            timeString.concat(".");
            timeString.concat(timeInfo.tm_year + 1900);
            timeString.concat(" ");
            timeString.concat(timeInfo.tm_hour);
            timeString.concat(":");
            timeString.concat(timeInfo.tm_min);
            timeString.concat(":");
            timeString.concat(timeInfo.tm_sec);
            return timeString;
        }  

        /**
         * @brief Returns date stamp with under scores for creating log filenames
         * 
         * @return String 
         */
        String fileDate() {
            struct tm timeInfo;
            if(!getLocalTime(&timeInfo)){
                return "Failed to obtain time";
            }
            String timeString = "";
            timeString.concat(timeInfo.tm_mday);
            timeString.concat("_");
            timeString.concat(timeInfo.tm_mon + 1);
            timeString.concat("_");
            timeString.concat(timeInfo.tm_year + 1900);
            return timeString;
        }   
};

/**
 * @brief A class for logging climate measurements to an SD card.
 * 
 */
class ClimateDataLogger {

    private:
        SDCard sdcard;
        ClimateTimeStamp time;
        String fileName;
        boolean rtcState;

    public:
        /**
         * @brief Construct a new ClimateDataLogger object for logging to an SD card.
         * 
         * @param ssid SSID of yout WiFi network
         * @param password password of yout WiFi network
         * @param rtcAlreadySet boolean, if the real time clock is already set
         */
        ClimateDataLogger(const char *ssid = "SSID",  const char *password = "PASSWORD", boolean rtcAlreadySet = false) {
                time = ClimateTimeStamp(ssid, password);
                fileName = "/log_";
                rtcState = rtcAlreadySet;
        }

        /**
         * @brief Initialises the SD card. If the real time clock is not set, it initialises the rtc. If no log file for the current
         * day is found, a new csv file is created.
         * 
         */
        void begin() {
            sdcard.begin();   
            if(!rtcState) {      
                time.setRealTimeClock();   
            }         
            fileName.concat(time.fileDate());
            fileName.concat(".csv");
            if(!sdcard.exists(fileName.c_str())) {
                sdcard.writeFile(fileName.c_str() , "time,temperature, humidity, pressure, pressureAtSealevel, height\n");
            }
        }

        /**
         * @brief Logs climate measurements to an SD card.
         * 
         * @param temperature current temperature
         * @param humidity current humidity
         * @param pressure current pressure
         * @param pressureAtSealevel calculated pressure at sealevel
         * @param height calculated height
         */
        void log(float temperature, float humidity, float pressure, float pressureAtSealevel, float height) {
            String data = time.getTimeStamp();
            data.concat(",");
            data.concat(temperature);
            data.concat(",");
            data.concat(humidity);
            data.concat(",");
            data.concat(pressure);
            data.concat(",");
            data.concat(pressureAtSealevel);
            data.concat(",");
            data.concat(height);
            data.concat("\n");
            sdcard.appendFile(fileName.c_str(), data.c_str());
        }
};

/**
 * @brief This class provides methods for reading the HTU21DF and the BMP085 sensor to aquire temperature, 
 * barometric pressure and humidity. This measurements can be logged on an SD card as a csv file.
 * 
 * @author Patrick Fock
 */
class ClimateSensor {

    private:
        Adafruit_HTU21DF humiditySensor;
        Adafruit_BMP085 barometricSensor;
        float referencePressure = 101325;
        ClimateDataLogger logger;
        const char* _ssid;
        const char* _password;

    public:
        /**
         * @brief Constructs a new ClimateSensor object.SSID and password are only required, when the RTC is not already set.
         * 
         * @param ssid SSID of your WiFi network
         * @param password password of your WiFi network
         */
        ClimateSensor(const char *ssid = "SSID",  const char *password = "PASSWORD") {
            _ssid = ssid;
            _password = password;
        }

        /**
         * @brief Initialises both sensors and starts the Datalogger, which saves the measurements to an SD Card.
         * 
         * @param rtcAlreadySet boolean, true if the real time clock is already set.
         * @return boolean success of initalisation
         */
        boolean begin(boolean rtcAlreadySet = false) {
            logger = ClimateDataLogger(_ssid, _password, rtcAlreadySet);
            logger.begin();
            return humiditySensor.begin() && barometricSensor.begin();
        }

        /**
         * @brief Reads the humidity using the HTU21DF sensor.
         * 
         * @return float humidity
         */
        float readHumidity() {
            return humiditySensor.readHumidity();
        }

        /**
         * @brief Approximates the real temperature by giving an average of the HTU21DF and BMP085 temperature readings.
         * 
         * @return float temperature
         */
        float readTemperature() {
            return (humiditySensor.readTemperature() + barometricSensor.readTemperature()) /2;
        }

        /**
         * @brief Reads the temperature using the HTU21DF sensor.
         * 
         * @return float temperature
         */
        float readTemperatureHTU21DF() {
            return humiditySensor.readTemperature();
        }

        /**
         * @brief Reads the temperature using the BMP180 sensor.
         * 
         * @return float temperature
         */
        float readTemperatureBMP085() {
            return barometricSensor.readTemperature();
        }

        /**
         * @brief Reads the pressure using the BMP180 sensor.
         * 
         * @return float pressure
         */
        float readPressure() {
            return barometricSensor.readPressure() / 100.0;
        }

        /**
         * @brief Calculates the pressure at sealevel.
         * 
         * @param altitude_meters height above sealevel of current position
         * @return float altitude at sealevel
         */
        float readSeaLevelPressure(float altitude_meters = 0) {
            return barometricSensor.readSealevelPressure(altitude_meters) / 100.0;
        }

        /**
         * @brief Set the reference height for calculating the sealevel pressure.
         * 
         * @param height height above sealevel of current position
         */
        void setReferenceHeight(float height) {
            referencePressure = readSeaLevelPressure(height) * 100;
        }

        /**
         * @brief Reads the altitude according to an given sealevel pressure. The default is 1013.25 mP.
         * 
         * @return float altitude
         */
        float readAltitude() {
            return barometricSensor.readAltitude(referencePressure);
        }

        /**
         * @brief Resets the HTU21DF with a 15 ms delay.
         */
         void resetHTU21DF() {
             humiditySensor.reset();
        }

        /**
         * @brief Creates or appends a csv log file for the measurements. The file is named in format "log_d_m_y.csv".
         * For example: "log_27_5_2022.csv"
         */
        void log() {
            logger.log(
                readTemperature(),
                readHumidity(),
                readPressure(),
                readSeaLevelPressure(readAltitude()),
                readAltitude()
            );
        }
};