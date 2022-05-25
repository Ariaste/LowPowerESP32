#include <SDCard.h>

class ClimateDataLogger {

    private:
        SDCard sdcard;

    public:
        void begin() {
            sdcard.begin();
            sdcard.writeFile("/log.csv", "temperature, humidity, pressure, pressureAtSealevel, height\n");
        }

        void log(float temperature, float humidity, float pressure, float pressureAtSealevel, float height) {
            String data = "";
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
            sdcard.appendFile("/log.csv", data.c_str());
        }
};        