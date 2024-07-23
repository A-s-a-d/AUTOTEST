#include "LCD2004.h"
#include <Wire.h>

LCD2004::LCD2004(/* args */) : lcd(adress_LCD, length_LCD, height_LCD)
{
}

void LCD2004::begin()
{
    Wire.begin();
    lcd.init();      // initialize the lcd
    lcd.backlight(); // open the backlight  //
    lcd.clear();
    lcd.setCursor(5, 0);    // move cursor to   (0, 0)
    lcd.print("AUTO-TEST"); // print message at (0, 0)
    lcd.setCursor(3, 1);    // move cursor to   (2, 1)
    lcd.print("INITIALISATION");
    delay(1000); // display the above for two seconds
    lcd.clear(); // clear display
}

void LCD2004::start_test()
{
    displayText_auto(_messages_start);
}

/* comment utiliser scroll TEXT:
  for (int y = 0; y < 4; y++) {
    scrollText(y, messages[y], startPositions[y]);
  }
*/
void LCD2004::scrollText(int y, const char *message, int &startPos)
{
    int len = strlen(message);
    lcd.setCursor(0, y);
    lcd.print("                    "); // Clear the line by printing 20 spaces
    lcd.setCursor(0, y);
    for (int x = 0; x < 20; x++)
    {
        int pos = (startPos + x) % len; // Calculate position in message
        lcd.print(message[pos]);
    }
    startPos = (startPos + 1) % len;
}

void LCD2004::mode_select(uint8_t mode)
{
    lcd.clear();
    lcd.setCursor(5, 0);      // move cursor to   (0, 0)
    lcd.print("SELECT MODE"); // print message at (0, 0)
    lcd.setCursor(3, 1);      // move cursor to   (2, 1)
    lcd.print("INITIALISATION");
    delay(1000); // display the above for two seconds
    lcd.clear(); // clear display
}

uint8_t LCD2004::calculate_center_x(const char *message)
{
    uint8_t len = strlen(message);
    return (20 - len) / 2;
}

void LCD2004::displayText_auto(const char *messages[])
{
    // lcd.clear();
    for (uint8_t y = 0; y < 4; y++)
    {
        uint8_t x = calculate_center_x(messages[y]);
        if (x <= 20)
        {
            lcd.setCursor(x, y);
            lcd.print(messages[y]);
        }
        else
        {
            scrollText(y, messages[y], _startPositions[y]);
        }
    }
}
