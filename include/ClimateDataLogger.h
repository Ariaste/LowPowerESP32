#include <SDCard.h>
#include <ClimateTimeStamp.h>

class ClimateDataLogger {

    private:
        SDCard sdcard;
        ClimateTimeStamp time;
        String fileName;

    public:

        ClimateDataLogger(const char *ssid = "SSID",  const char *password = "PASSWORD") {
                time = ClimateTimeStamp(ssid, password);
                fileName = "/log_";
        }

        void begin() {
            sdcard.begin();        
            time.setRealTimeClock();           
            fileName.concat(time.fileDate());
            fileName.concat(".csv");
            if(!sdcard.exists(fileName.c_str())) {
                sdcard.writeFile(fileName.c_str() , "time,temperature, humidity, pressure, pressureAtSealevel, height\n");
            }
        }

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