#include <SDCard.h>
#include <ClimateTimeStamp.h>

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