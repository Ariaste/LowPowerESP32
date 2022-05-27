#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

/**
 * @brief A class for handeling SPI SD card modules.
 * 
 * @author Patrick Fock 
 */
class SDCard {

    public:

        /**
         * @brief Initialises the SD card.
         * 
         * @return success/failure of initialisation
         */
        boolean begin() {

            Serial.begin(115200);
            if(!SD.begin(5)){
                Serial.println("Card Mount Failed");
                return false;
            }
            uint8_t cardType = SD.cardType();

            if(cardType == CARD_NONE){
                Serial.println("No SD card attached");
                return false;
            }

            String types[4] = {"MMC", "SDSC", "SDHC", "UNKNOWN"};
            Serial.printf(
                "\nSD Card Type: %s\nSD Card Size: %llu MB\n", 
                types[cardType > 0 && cardType <= 3 ? cardType - 1 : 3],
                SD.cardSize() / (1024 * 1024)
            );           
            return true;
    } 

        /**
         * @brief list given directory and given levels of subdirectories
         * 
         * @param dirname directory to list
         * @param levels number of subdectory levels
         */
        void listDir(const char * dirname, uint8_t levels) {

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
         * @return success/failure of creation
         */
        boolean createDir(const char * path) {
            return SD.mkdir(path);
}

        /**
         * @brief Removes a directory.
         * 
         * @param path path of directory to be removed.
         * @return success/failure of removing
         */
        boolean removeDirectory(const char * path) {
            return SD.rmdir(path);
        }

        /**
         * @brief Reads a file and prints it to the console.
         * 
         * @param path file path
         */
        void printFile(const char * path) {

            File file = SD.open(path);
            if(!file){
                Serial.printf("Failed to open %s for reading", path);
                return;
            }

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
        String readFileToString(const char * path) {
            String fileAsString = "";

            File file = SD.open(path);
            if(!file){
                Serial.printf("Failed to open %s for reading", path);
                return fileAsString;
            }

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
        File readFile(const char * path) {
            File file = SD.open(path);
            if(!file){
                Serial.printf("Failed to open %s for reading", path);
            }
            return file;
        }

        /**
         * @brief Checks if a file exists.
         * 
         * @param path file path
         * @return boolean true if file exists else false
         */
        boolean exists(const char* path) {
            return SD.exists(path);
        }

        /**
         * @brief Writes to a file
         * 
         * @param path file path
         * @param content file content
         * @return success/failure of writing
         */
        boolean writeFile(const char * path, const char * content) {
            boolean success;
            File file = SD.open(path, FILE_WRITE);
            if(!file){
                return false;
            }
            if(file.print(content)){
                success = true;
            } else {
                success = false;
            }
            file.close();
            return success;
        }

        /**
         * @brief Appends data to a file.
         * 
         * @param path file path
         * @param content content to append
         * @return success/failure of appending
         */
        boolean appendFile(const char * path, const char * content){
            File file = SD.open(path, FILE_APPEND);
            if(!file){
                return false;
            }
            if(!file.print(content)){
                return false;
            }
            file.close();
            return true;
        }

        /**
         * @brief Renames a file.
         * 
         * @param oldPath old name including path
         * @param newPath new name including path
         * @return success/failure of renaming
         */
        boolean renameFile(const char * oldPath, const char * newPath){
            return SD.rename(oldPath, newPath);
        }

        /**
         * @brief Deletes a file.
         * 
         * @param path file to delete
         * @return success/failure of deletion
         */
        boolean deleteFile(const char * path){
            return SD.remove(path);
        }

};