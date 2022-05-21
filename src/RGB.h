#include <Arduino.h>

/**
 * @brief A class for controlling rgb leds.
 * 
 */
class RGB {

    public:

        /**
         * @brief Construct a new RGB object.
         * 
         * @param redPin pin of red led
         * @param greenPin pin of green led
         * @param bluePin pin of blue led
         * @param frequency pwm frequency
         * @param resolution pwm resolution, up to 12 bit
         */
        RGB(const int redPin, int greenPin, int bluePin, int frequency = 5000, int resolution = 8) {
            ledcSetup(0, frequency, resolution);
            ledcAttachPin(redPin, 0);
            ledcSetup(1, frequency, resolution);
            ledcAttachPin(greenPin, 1);
            ledcSetup(2, frequency, resolution);
            ledcAttachPin(bluePin, 2);
            clear();
        }

        /**
         * @brief Set the led color
         * 
         * @param red red value
         * @param green green value
         * @param blue blue value
         */
        void setColor(int red, int green, int blue) {
            clear();
            ledcWrite(0, red);
            ledcWrite(1, green);
            ledcWrite(2, blue);
        }

        /**
         * @brief Turns led off.
         * 
         */
        void clear() {
            ledcWrite(0, 0);
            ledcWrite(1, 0);
            ledcWrite(2, 0);
        }

        /**
         * @brief Creates a cycle of red, green and blue.
         * 
         */
        void testCycle() {
            int t = 1000;
            while(true) {
                clear();
                setColor(255, 0, 0);
                delay(t);
                clear();
                setColor(0, 255, 0);
                delay(t);
                clear();
                setColor(0, 0, 255);
                delay(t);
            }
        }
};