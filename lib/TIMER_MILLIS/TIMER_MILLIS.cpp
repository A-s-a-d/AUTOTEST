#include "TIMER_MILLIS.h"

TIMER_MILLIS::TIMER_MILLIS(uint32_t time_ms, void (*callback)(void))
{
    period = time_ms;
    callback_function = callback;
}

void TIMER_MILLIS::begin()
{
    startMillis = millis(); // initial start time
}

void TIMER_MILLIS::update()
{
    // Serial.print("here");

    if (millis() - startMillis >= period) // test whether the period has elapsed
    {
        startMillis = millis(); // IMPORTANT to save the start time of the current LED state.
        callback_function();
    }
}