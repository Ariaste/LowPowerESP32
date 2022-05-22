#include <SDESP32.h>
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

/**
 * @brief A class for handeling SPI SD card modules
 * 
 * @author Patrick Fock 
 */
class SDCard {

    private:
        
    public:
        void begin() {

            Serial.begin(115200);
            if(!SD.begin(5)){
                Serial.println("Card Mount Failed");
                return;
            }
            uint8_t cardType = SD.cardType();

            if(cardType == CARD_NONE){
                Serial.println("No SD card attached");
                return;
            }

            Serial.print("SD Card Type: ");
            if(cardType == CARD_MMC){
                Serial.println("MMC");
            } else if(cardType == CARD_SD){
                Serial.println("SDSC");
            } else if(cardType == CARD_SDHC){
                Serial.println("SDHC");
            } else {
                Serial.println("UNKNOWN");
            }

            uint64_t cardSize = SD.cardSize() / (1024 * 1024);
            Serial.printf("SD Card Size: %llu MB\n", cardSize);
    } 

    void listDir(const char * dirname, uint8_t levels){
        Serial.printf("Listing directory: %s\n", dirname);

        File root = SD.open(dirname);
        if(!root){
            Serial.println("Failed to open directory");
            return;
        }
        if(!root.isDirectory()){
            Serial.println("Not a directory");
            return;
        }

        File file = root.openNextFile();
        while(file){
            if(file.isDirectory()){
            Serial.printf("  DIR : %s\n", file.name());
            if(levels){
                listDir(file.name(), levels -1);
            }
            } else {
            Serial.printf("  FILE: %s SIZE: %d B\n", file.name(), file.size());
            }
            file = root.openNextFile();
        }
    }    
};