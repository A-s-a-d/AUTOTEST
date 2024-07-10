#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define PIN_START 25 // MARCHE
#define PIN_STOP 26  // ARRET

/* by default STOP button is HIGH and START button is LOW
when pressed Stop becomes LOW
when pressed START becomes HIGH
 */

extern volatile boolean START;
extern volatile boolean STOP;

class BUTTON
{
private:
    static void IRAM_ATTR START_pressed_ISR();
    static void IRAM_ATTR STOP_pressed_ISR();

public:
    BUTTON();
    void begin_interruption();
    boolean process_data();
};

#endif