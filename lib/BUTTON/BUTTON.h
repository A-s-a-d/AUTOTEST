#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define PIN_START 25 // MARCHE
#define PIN_STOP 26  // ARRET

extern volatile boolean START;
extern volatile boolean STOP;

extern volatile boolean start  ; // Initially true because it's normally closed (HIGH)
extern volatile boolean stop  ;  // Initially true because it's normally closed (HIGH)

class BUTTON
{
private:
    static void IRAM_ATTR START_pressed_ISR();
    static void IRAM_ATTR STOP_pressed_ISR();

public:
    BUTTON();
    void begin_interruption();
    void update();
};

#endif
