#include <Arduino.h>

    /**
     * @brief Enables the deep sleep mode of the ESP32 and wakes it up on based on a timer.
     * 
     * @param seconds time to sleep in seconds
     */
    void deepSleep(int seconds) {
        esp_sleep_enable_timer_wakeup(seconds * 1000000);
        esp_deep_sleep_start();
    }

    /**
     * @brief Enables the deep sleep mode of the ESP32 and wakes it up on based on a timer.
     * 
     * @param seconds time to sleep in minutes
     */
    void deepSleep(int minutes) {
        esp_sleep_enable_timer_wakeup(minutes * 60000000);
        esp_deep_sleep_start();
    }

    /**
     * @brief Enables the deep sleep mode of the ESP32 and wakes it up on based on a timer.
     * 
     * @param seconds time to sleep in hours
     */
    void deepSleep(int hours) {
        esp_sleep_enable_timer_wakeup(hours * 3600000000);
        esp_deep_sleep_start();
    }


