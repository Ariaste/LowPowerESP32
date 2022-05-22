#include <Arduino.h>

    const int SECONDS = 1000000;
    const int MINUTES = 60000000;
    const int HOURS = 3600000000;

    /**
     * @brief Enables deepsleep of the ESP32 and wakes it up after the given time.
     * 
     * @param time amount of time units to sleep
     * @param mode unit of time, available: SECONDS, MINUTES, HOURS
     */
    void deepSleep(int time, int unit) {
        esp_sleep_enable_timer_wakeup(time * unit);
        esp_deep_sleep_start();
    }

    /**
     * @brief Enables deepsleep of the ESP32 and wakes it up after the given time.
     * 
     * @param seconds wake up after given seconds
     */
    void deepSleepForSeconds(int seconds) {
        deepSleep(seconds, SECONDS);
    }

    /**
     * @brief Enables deepsleep of the ESP32 and wakes it up after the given time.
     * 
     * @param seconds wake up after given minutes
     */
    void deepSleepForMinutes(int minutes) {
        deepSleep(minutes, MINUTES);
    }

    /**
     * @brief Enables deepsleep of the ESP32 and wakes it up after the given time.
     * 
     * @param seconds wake up after given hours
     */
    void deepSleepForHours(int hours) {
        deepSleep(hours, HOURS);
    }