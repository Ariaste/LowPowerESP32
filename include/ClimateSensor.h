#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP085.h>
#include <cmath>

/** This class unites the HTD21DF and the BMP085 sensor. */
class ClimateSensor {

    private:
        Adafruit_HTU21DF humiditySensor;
        Adafruit_BMP085 barometricSensor;
        float referencePressure = 101325;

    public:

        /**
         * Initialising both sensors.
         * @return bool if initialisation successful.
         */ 
        boolean begin() {
            return humiditySensor.begin() && barometricSensor.begin();
        }

        /**
         * Reads the humidity using the HTU21DF sensor.
         * @return float humidity
         */
        float readHumidity() {
            return humiditySensor.readHumidity();
        }

        /**
         * Approximates the real temperature by giving an average of the HTU21DF and BMP085 temperature readings.
         * @return float temperature
         */
        float readTemperature() {
            return (humiditySensor.readTemperature() + barometricSensor.readTemperature()) /2;
        }

        /**
         * Reads the temperature using the HTU21DF sensor.
         * @return float temperature
         */
        float readTemperatureHTU21DF() {
            return humiditySensor.readTemperature();
        }

        /**
         * Reads the temperature using the BMP180 sensor.
         * @return float temperature
         */
        float readTemperatureBMP085() {
            return barometricSensor.readTemperature();
        }

        /**
         * Reads the pressure using the BMP180 sensor.
         * @return float pressure
         */
        float readPressure() {
            return barometricSensor.readPressure() / 100.0;
        }

        /**
         * Calculates the pressure at sealevel.
         * @return float altitude at sealevel
         */
        float readSeaLevelPressure(float altitude_meters = 0) {
            return barometricSensor.readSealevelPressure(altitude_meters) / 100.0;
        }

        void setReferenceHeight(float height) {
            referencePressure = readSeaLevelPressure(height) * 100;
        }

        /**
         * Reads the altitude according to an given sealevel pressure. The default is 1013.25 mP.
         * @return float altitude
         */
        float readAltitude() {
            return barometricSensor.readAltitude(referencePressure);
        }

        /**
         * Resets the HTU21DF with a 15 ms delay.
         */
         void resetHTU21DF() {
             humiditySensor.reset();
         }
        
};
