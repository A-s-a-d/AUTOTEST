#ifndef DEBUG_TIMING_MAIN_H
#define DEBUG_TIMING_MAIN_H
#include "debug_variable.h"
#include <Arduino.h>

inline void debugStartTime(unsigned long &startLoop_time)
{
    startLoop_time = esp_timer_get_time();
}

inline void debugEndTime(unsigned long &startLoop_time)
{
    unsigned long endLoop = esp_timer_get_time();
    Serial.println(endLoop - startLoop_time);
    Serial.print(" µs");
}

#endif