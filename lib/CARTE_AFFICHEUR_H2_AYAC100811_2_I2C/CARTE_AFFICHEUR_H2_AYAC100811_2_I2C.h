
/*!
 * @file CARTE_AFFICHEUR_H2_AYAC100811_2_I2C.h
 * @brief to control the display circuit and buttons.
 * @brief LCD 1602 configuration 6800 -> I2C and buttons too in I2C
 */

/*
 */
#ifndef CARTE_AFFICHEUR_H2_AYAC100811_2_I2C_H
#define CARTE_AFFICHEUR_H2_AYAC100811_2_I2C_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "global.h"
// #include "..\definitions.h"

#define backlight_pin 12
#define ON 0
#define OFF 1

#define number_of_lines 2
#define number_of_rows 16

class CARTE_AFFICHEUR_H2_AYAC100811_2_I2C
{
private:
    Adafruit_RGBLCDShield lcd;
    uint8_t _mode;
    uint8_t _button;

    // see boolean print_verify(const String string, uint8_t col, uint8_t row);
    // _previousText Previous text buffer for each row.
    String _previousText[2] = {"", ""};

    uint8_t _mode_selected();

    int _startPositions[2] = {0, 0};

public:
    CARTE_AFFICHEUR_H2_AYAC100811_2_I2C(/* args */);
    uint8_t blinkx = 0;

    /**
     * @enum mode
     * @brief Modes for test
     *
     * This enum defines the different modes for the test.
     */

    void begin();
    /*!
     * @brief buttons()
     * @return returns a value depending on the button pressed.
     * @retval BUTTON_UP = 0x08 returns 1
     * @retval BUTTON_DOWN = 0x04 returns 2
     * @retval BUTTON_LEFT = 0x10 returns 3
     * @retval BUTTON_RIGHT = 0x02 returns 4
     * @retval BUTTON_SELECT = 0x01 returns 5
     */
    uint8_t last_buttons();
    uint8_t pressed_button();
    /*!
     * @brief back_light to turn on or off the back light of the LCD on circuit AYAC100811_2 hardware 2
     * @param state either "ON"/1 or "OFF"/0.
     */
    void back_light(boolean state);

    /*!
     *@brief setcursor to give access to the function setcursor of object lcd
     * @param col colone number
     * @param row row number
     */
    void setcursor(uint8_t col, uint8_t row);

    /*!
     *@brief print to give access to the function print of object lcd
     * @param string string that needs to be displayed
     */
    void print(const String string);

    /**
     * @brief print_verify , it varifies if the the text that is been asked to print is different
     * then the previous text, if it's true only then the new text is displayed.
     *
     * @param string the string that needs to be written, @c string-must-be-16-char
     * @param col colone number
     * @param row row number
     *
     * @return printed = 1 if the text has been printed if 0 means the text was same and hasn't been printed.
     *
     */
    boolean print_verify(const String string, uint8_t col, uint8_t row);

    String center_x(const String &inputString);

    void scrollText(uint8_t y, String message, int &startPos);

    void clear();

    /*!
     * @brief select_device -  selecting the device at the start
     * @attention Must be used inside the setup, and a device MUST BE chosen.
     */
    void select_device();
    uint8_t device_selected();

    /*!
     * @brief select_mode - select the test mode at the start
     * @attention Must be used inside the setup after select_device, and a mode MUST BE chosen.
     */
    void select_mode();
    uint8_t mode_selected();

    /*!
     * @brief test_ongoing - function to display that a test cycle is in progress
     */
    void test_ongoing(boolean status);

    void start_test(); // display text on how to start test.
    void mode_select(uint8_t mode);
    void displayText_auto(const char *messages[]);
};

#endif