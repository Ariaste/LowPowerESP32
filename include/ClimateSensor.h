#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP085.h>
#include <cmath>
#include <ClimateDataLogger.h>

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
