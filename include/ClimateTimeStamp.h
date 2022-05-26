#include <Arduino.h>
#include <WiFi.h>
#include <time.h>



class ClimateTimeStamp {

    private:
        const char* _ssid;
        const char* _password;
        const char* _ntpServer;
        long _gmtOffset_sec = 0;
        int _daylightOffset_sec = 3600;

    public:

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

