#include <Arduino.h>

/**
 * @brief A class for controlling rgb leds.
 * @author Patrick Fock
 */
class RGB {

    public:

        /**
         * @brief Constants of different colors. To use them, call setColor(<name of constant>).
         * 
         */
        const int RED = 0xFF0000;
        const int GREEN = 0x00FF00;
        const int BLUE = 0x0000FF;
        const int YELLOW = 0xFFFF00;
        const int WHITE = 0xFFFFFF;
        const int PINK = 0xF207BF;

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
         * @brief Set the led color.
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
         * @brief Set the led color.
         * 
         * @param rgb integer containing red, green and blue value.
         */
        void setColor(int rgb) {
            int red = (rgb >> 16) % 256;
            int green = (rgb >> 8) % 256;
            int blue = rgb % 256;
            setColor(red, green, blue);
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
         * @brief Creates a cycle of colours.
         * 
         */
        void testCycle(int t) {
            int colours[] = {RED, GREEN, BLUE, YELLOW, PINK, WHITE};
            while(true) {
                for(int colour : colours) {
                    setColor(colour);
                    delay(t);
                }
            }
        }
};