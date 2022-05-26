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
            Serial.printf("Creating Dir: %s\n", path);
            if(SD.mkdir(path)){
                Serial.println("Dir created");
                return true;
            } else {
                Serial.println("mkdir failed");
                return false;
        }
}

        /**
         * @brief Removes a directory.
         * 
         * @param path path of directory to be removed.
         * @return success/failure of removing
         */
        boolean removeDir(const char * path) {
            Serial.printf("Removing Dir: %s\n", path);
            if(SD.rmdir(path)){
                Serial.println("Dir removed");
                return true;
            } else {
                Serial.println("rmdir failed");
                return false;
            }
        }

        /**
         * @brief Reads a file and prints it to the console.
         * 
         * @param path file path
         */
        void printFile(const char * path) {
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
        String readFileToString(const char * path) {
            String fileAsString = "";
            Serial.printf("Reading file: %s\n", path);

            File file = SD.open(path);
            if(!file){
                Serial.println("Failed to open file for reading");
                return fileAsString;
            }

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
        File readFile(const char * path) {
            Serial.printf("Reading file: %s\n", path);

            File file = SD.open(path);
            if(!file){
                Serial.println("Failed to open file for reading");
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
            return SD.open(path);
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
            Serial.printf("Writing file: %s\n", path);

            File file = SD.open(path, FILE_WRITE);
            if(!file){
                Serial.println("Failed to open file for writing");
                return false;
            }
            if(file.print(content)){
                Serial.println("File written");
                success = true;
            } else {
                Serial.println("Write failed");
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
            Serial.printf("Renaming file %s to %s\n", oldPath, newPath);
            if (SD.rename(oldPath, newPath)) {
                Serial.println("File renamed");
                return true;
            } else {
                Serial.println("Rename failed");
                return false;
            }
        }

        /**
         * @brief Deletes a file.
         * 
         * @param path file to delete
         * @return success/failure of deletion
         */
        boolean deleteFile(const char * path){
            Serial.printf("Deleting file: %s\n", path);
            if(SD.remove(path)){
                Serial.println("File deleted");
                return true;
            } else {
                Serial.println("Delete failed");
                return false;
            }
        }

};