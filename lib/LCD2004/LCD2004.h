#ifndef LCD2004_H
#define LCD2004_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define adress_LCD 0x27
#define length_LCD 20
#define height_LCD 4
class LCD2004
{
private:
    LiquidCrystal_I2C lcd;
    const char *_messages_start[4] = {
        "Allumer simulateur",
        "Brancher le bnc",
        "Mettre le boitier",
        "Appuyer sur le button pour demarrer"};
    int _startPositions[4] = {0, 0, 0, 0};

public:
    LCD2004(/* args */);
    void begin();
    void scrollText(int line, const char *message, int &startPos);
    void mode_select(uint8_t mode);
    uint8_t calculate_center_x(const char *message);
    void displayText_auto(const char *messages[]);
};

#endif