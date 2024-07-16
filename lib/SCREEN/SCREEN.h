#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define adress_LCD 0x27
#define length_LCD 20
#define height_LCD 4
class SCREEN
{
private:
    LiquidCrystal_I2C lcd;
    const char *_messages[4] = {
        "Allumer le simulateur    ",
        "Brancher le bnc    ",
        "Mettre le boitier en place    ",
        "Appuyer sur le button pour demarrer    "};
    int _startPositions[4] = {0, 0, 0, 0};

public:
    SCREEN(/* args */);
    void begin();
    void start_test(); // display text on how to start test.
void scrollText(int line, const char* message, int& startPos);
};

#endif