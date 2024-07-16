#include "SCREEN.h"
#include <Wire.h>

SCREEN::SCREEN(/* args */) : lcd(adress_LCD, length_LCD, height_LCD)

{
}

void SCREEN::begin()
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

void SCREEN::start_test()
{
    for (int line = 0; line < 4; line++)
    {
        scrollText(line, _messages[line], _startPositions[line]);
    }
}

/* comment utiliser scroll TEXT:
  for (int line = 0; line < 4; line++) {
    scrollText(line, messages[line], startPositions[line]);
  }
*/
void SCREEN::scrollText(int line, const char *message, int &startPos)
{
    int len = strlen(message);
    // Clear the specified line
    lcd.setCursor(0, line);
    lcd.print("                    "); // Clear the line by printing 20 spaces

    // Set cursor to the start of the specified line
    lcd.setCursor(0, line);

    // Print the message starting from the current start position
    for (int i = 0; i < 20; i++)
    {
        int pos = (startPos + i) % len; // Calculate position in message
        lcd.print(message[pos]);
    }

    // Update the start position for the next scroll
    startPos = (startPos + 1) % len;
}
