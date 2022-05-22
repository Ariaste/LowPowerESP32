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

        /**
         * @brief Initialises the SD card.
         * 
         */
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

        /**
         * @brief list given directory and given levels of subdirectories
         * 
         * @param dirname directory to list
         * @param levels number of subdectory levels
         */
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

        /**
         * @brief List current directory.
         * 
         */
        void listDir() {
            listDir("/", 0);
        } 

        /**
         * @brief Create a new directory.
         * 
         * @param path path of the new directory
         */
        void createDir(const char * path){
            Serial.printf("Creating Dir: %s\n", path);
            if(SD.mkdir(path)){
                Serial.println("Dir created");
            } else {
                Serial.println("mkdir failed");
        }
}

        /**
         * @brief Removes a directory.
         * 
         * @param path path of directory to be removed.
         */
        void removeDir(const char * path){
            Serial.printf("Removing Dir: %s\n", path);
            if(SD.rmdir(path)){
                Serial.println("Dir removed");
            } else {
                Serial.println("rmdir failed");
            }
        }

        /**
         * @brief Reads a file and prints it to the console.
         * 
         * @param path file path
         */
        void printFile(const char * path){
            Serial.printf("Reading file: %s\n", path);

            File file = SD.open(path);
            if(!file){
                Serial.println("Failed to open file for reading");
                return;
            }

            Serial.print("Read from file: ");
            while(file.available()){
                Serial.write(file.read());
            }
            file.close();
        }

        /**
         * @brief Reads a file and return the content as String.
         * 
         * @param path file to read
         * @return String file content
         */
        String readFileToString(const char * path){
            Serial.printf("Reading file: %s\n", path);

            File file = SD.open(path);
            if(!file){
                Serial.println("Failed to open file for reading");
                return;
            }

            String fileAsString = "";

            Serial.print("Read from file: ");
            while(file.available()){
                fileAsString += file.read() + "\n";
            }
            file.close();
            return fileAsString;
        }

        /**
         * @brief Reads a file and return a File object.
         * 
         * @param path file to read
         * @return File file object
         */
        File readFile(const char * path){
            Serial.printf("Reading file: %s\n", path);

            File file = SD.open(path);
            if(!file){
                Serial.println("Failed to open file for reading");
                return;
            }

            return file;
        }

};