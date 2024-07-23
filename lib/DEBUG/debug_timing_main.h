#ifndef DEBUG_TIMING_MAIN_H
#define DEBUG_TIMING_MAIN_H
#include "debug_variable.h"


inline void debugStartTime(unsigned long &startLoop_time)
{
    startLoop_time = micros();
}

inline void debugEndTime(unsigned long &startLoop_time)
{
    unsigned long endLoop = micros();
    Serial.print("Loop ");
    Serial.println(endLoop - startLoop_time);
}

#endif