/*
LCD 1602 configuration 6800 -> I2C avec les buttons en I2C aussi
*/
#include "CARTE_AFFICHEUR_H2_AYAC100811_2_I2C.h"
// included inside .hfile
//  #include <Arduino.h>
//  #include <Wire.h>
//  #include <Adafruit_RGBLCDShield.h>
//  #include <utility/Adafruit_MCP23017.h>
//  #include "global.h"

CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::CARTE_AFFICHEUR_H2_AYAC100811_2_I2C(/* args */) : lcd()
{
    _mode = 0;
    _button = 0;
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::begin()
{

    lcd.begin(16, 2);

    lcd.setCursor(4, 0);
    lcd.print("AUTO-TEST");
    delay(1000);
    pinMode(backlight_pin, OUTPUT);
    back_light(ON);
    lcd.clear();
    select_device();
    select_mode();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("AUTO-TEST");
}

uint8_t CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::last_buttons()
{
    uint8_t buttons = lcd.readButtons();
    static uint8_t reply = 0;
    switch (buttons)
    {
    case BUTTON_LEFT:
        reply = 1;
        // Serial.println("LEFT");
        break;
    case BUTTON_UP:
        reply = 2;
        // Serial.println("UP");
        break;
    case BUTTON_DOWN:
        reply = 3;
        // Serial.println("DOWN");
        break;
    case BUTTON_RIGHT:
        reply = 4;
        // Serial.println("RIGHT");
        break;
    case BUTTON_SELECT:
        reply = 5;
        // Serial.println("SELECT");
        break;
    default:
        break;
    }
    return reply;
}

uint8_t CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::pressed_button()
{
    static unsigned long buttonPressStart = 0; // Time when the button was first pressed
    static unsigned long lastReplyTime = 0;    // Time when the last reply was sent
    static uint8_t lastButtonState = 0;        // Last button state
    uint8_t buttons = lcd.readButtons();
    uint8_t reply = 0;
    unsigned long currentTime = millis();

    // Check if any button is pressed
    if (buttons != 0)
    {
        // If button state has changed (new button press)
        if (buttons != lastButtonState)
        {
            buttonPressStart = currentTime; // Record the time when the button was first pressed
            lastButtonState = buttons;
        }

        // Check if 2 seconds have passed since the last reply
        if (currentTime - lastReplyTime >= 500)
        {
            // Determine reply based on button pressed
            switch (buttons)
            {
            case BUTTON_LEFT:
                reply = 1;
                // Serial.println("LEFT");
                break;
            case BUTTON_UP:
                reply = 2;
                // Serial.println("UP");
                break;
            case BUTTON_DOWN:
                reply = 3;
                // Serial.println("DOWN");
                break;
            case BUTTON_RIGHT:
                reply = 4;
                // Serial.println("RIGHT");
                break;
            case BUTTON_SELECT:
                reply = 5;
                // Serial.println("SELECT");
                break;
            default:
                break;
            }
            // Update the time of the last reply
            lastReplyTime = currentTime;
        }
        else
        {
            // If not enough time has passed, send 0
            reply = 0;
        }
    }
    else
    {
        // Reset when button is released
        lastButtonState = 0;
        reply = 0;
    }

    return reply;
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::back_light(boolean state)
{
    state == ON ? digitalWrite(backlight_pin, ON) : digitalWrite(backlight_pin, OFF);
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::setcursor(uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::print(const String string)
{
    lcd.print(string);
}

boolean CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::print_verify(const String string, uint8_t col, uint8_t row)
{
    String textToPrint = string;

    // Ensure the text is at least 16 characters long
    if (textToPrint.length() < 16)
    {
        // Append spaces to make it 16 characters long
        while (textToPrint.length() < 16)
        {
            textToPrint += " ";
        }
    }

    // Only update if the new text is different from the previous text
    if (_previousText[row] != textToPrint)
    {
        lcd.setCursor(col, row);
        lcd.print(textToPrint);
        _previousText[row] = textToPrint; // Update the buffer
        return true;                      // Indicate that an update occurred
    }

    return false; // Indicate that no update was necessary
}

uint8_t CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::mode_selected()
{
    return _mode;
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::select_device()
{
    back_light(ON);
    static bool cleared = false;
    static uint8_t currentDevice = 0; // Track current device index
    static uint8_t previousDevice = 0;
    static uint8_t selectedDevice;
    static uint8_t button = 0;

    lcd.clear();

    while (!device_has_been_selected)
    {
        button = pressed_button(); // last_buttons() returns uint8_t
                                   // Serial.println(button);

        lcd.setCursor(0, 0);
        lcd.print("DEVICE: ");
        lcd.setCursor(6, 1);
        lcd.print(DEVICE_NAMES[currentDevice]);
        // Serial.println(DEVICE_NAMES[currentDevice]);

        // Handle button presses for navigation and selection
        if (button == 1)
        {                                                      // Button 1 for next device
            currentDevice = (currentDevice + 1) % MAX_DEVICES; // modulo returns to first device if currentDevice = MAX_DEVICES
            cleared = false;                                   // Reset cleared flag
            if (currentDevice != previousDevice)
            {
                lcd.clear();
                previousDevice = currentDevice;
            }
            // Display current device name
        }
        else if (button == 5)
        { // Button 5 for selection
            if (!cleared)
            {
                lcd.clear();
                cleared = true;
            }
            selectedDevice = currentDevice;
            device_has_been_selected = true;
            myDEVICE.SET_DEVICE(currentDevice);
            lcd.setCursor(1, 0);
            lcd.print("DEVICE SELECTED: ");
            lcd.setCursor(6, 1);
            lcd.print(DEVICE_NAMES[currentDevice]);
            delay(1000);
        }
    }
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::select_mode()
{
    back_light(ON);
    static bool cleared = false;
    static uint8_t currentMode = 0; // Track current device index
    static uint8_t previousMode = 0;
    static uint8_t selectedMode;
    static uint8_t button = 0;
    lcd.clear();
    while (!mode_has_been_selected)
    {
        button = pressed_button(); // last_buttons() returns uint8_t
        // Serial.println(button);

        lcd.setCursor(0, 0);
        lcd.print("MODE: ");
        lcd.setCursor(6, 1);
        lcd.print(MODE_NAMES[myDEVICE.Device_ID][currentMode]);
        // Serial.println(MODE_NAMES[myDEVICE.Device_ID][currentMode]);

        // Handle button presses for navigation and selection
        if (button == 1)                                                // Update current device name
        {                                                               // Button 1 for next device
            currentMode = (currentMode + 1) % myDEVICE.number_of_modes; // modulo returns to first device if currentDevice = MAX_DEVICES
            cleared = false;                                            // Reset cleared flag
            if (currentMode != previousMode)
            {
                lcd.clear();
                previousMode = currentMode;
            }
        }
        else if (button == 5)
        { // Button 5 for selection
            if (!cleared)
            {
                lcd.clear();
                cleared = true;
            }
            selectedMode = currentMode;
            mode_has_been_selected = true;
            myMODE.Mode_ID = selectedMode;
            myMODE.Mode_name = MODE_NAMES[myDEVICE.Device_ID][selectedMode];
            lcd.setCursor(1, 0);
            lcd.print("MODE SELECTED: ");
            lcd.setCursor(6, 1);
            lcd.print(MODE_NAMES[myDEVICE.Device_ID][selectedMode]);
            delay(1000);
        }
    }
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::clear()
{
    static bool cleared = false;
    if (!cleared)
    {
        lcd.clear();
        cleared = true;
    }
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::test_ongoing(boolean status)
{
    static boolean lastStatus = false;

    if (status != lastStatus)
    {
        lastStatus = status;
        lcd.clear();
        if (status)
        {
            lcd.setCursor(0, 0);
            lcd.print("TEST ONGOING   ");
            lcd.setCursor(0, 1);
            lcd.print("               ");
        }
        else
        {
            lcd.setCursor(0, 0);
            lcd.print("TEST ENDED or  ");
            lcd.setCursor(0, 1);
            lcd.print("WAITING START  ");
        }
    }
}

void CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::mode_select(uint8_t mode)
{
    lcd.clear();
    lcd.setCursor(5, 0);      // move cursor to   (0, 0)
    lcd.print("SELECT MODE"); // print message at (0, 0)
    lcd.setCursor(3, 1);      // move cursor to   (2, 1)
    lcd.print("INITIALISATION");
    delay(1000); // display the above for two seconds
    lcd.clear(); // clear display
}

String CARTE_AFFICHEUR_H2_AYAC100811_2_I2C::center_x(const String &inputString)
{
    uint8_t len = inputString.length(); // Length of the input string

    // Calculate the number of leading spaces needed to center the text
    if (len >= number_of_rows)
    {
        // If the string is longer than or equal to the screen width, return it truncated to fit
        return inputString.substring(0, number_of_rows);
    }
    uint8_t totalSpaces = number_of_rows - len;
    uint8_t spacesOnEachSide = totalSpaces / 2;
    uint8_t extraSpace = totalSpaces % 2; // Handle odd number of spaces
    String centeredString = "";
    // Add leading spaces
    for (uint8_t i = 0; i < spacesOnEachSide; i++)
    {
        centeredString += " ";
    }
    // Add the input string
    centeredString += inputString;
    // Add trailing spaces if needed
    for (uint8_t i = 0; i < spacesOnEachSide + extraSpace; i++)
    {
        centeredString += " ";
    }

    return centeredString;
}

