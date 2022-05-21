#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP085.h>

/** This class unites the HTD21DF and the BMP180 sensor. */
class ClimateSensor {

    private:
        Adafruit_HTU21DF humiditySensor;
        Adafruit_BMP085 barometricSensor;

    public:
        ClimateSensor();

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
         * Approximates the real temperature by giving an average of the HTU21DF and BMP180 temperature readings.
         * @return float temperature
         */
        float readTemperature() {
            return (humiditySensor.readTemperature() + barometricSensor.readTemperature()) /2;
        }

        /**
         * 
         */
        float readTemperatureHTU21DF() {
            return humiditySensor.readTemperature();
        }

        /**
         * 
         */
        float readTemperatureBMP180() {
            return barometricSensor.readTemperature();
        }

        /**
         * 
         */
        float readPressure() {
            return barometricSensor.readPressure();
        }

        /**
         * 
         */
        float readAltitude(float sealevelPressure=101325) {
            return barometricSensor.readAltitude(sealevelPressure);
        }
        
};
