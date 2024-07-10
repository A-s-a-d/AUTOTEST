#ifndef TIMER_MILLIS_H
#define TIMER_MILLIS_H

#include <Arduino.h>

class TIMER_MILLIS
{
private:
    unsigned long startMillis; // some global variables available anywhere in the program
    unsigned long currentMillis;
    unsigned long period; // the value is a number of milliseconds
    void (*callback_function)(void);

public:
    TIMER_MILLIS(uint32_t time_ms, void (*callback)(void));
    void begin();
    void update();
};

#endif
