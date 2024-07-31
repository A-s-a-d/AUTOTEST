#include "debug_timing_main.h"
unsigned long accumulatedTime = 0;
unsigned long loopCount = 0;
unsigned long lastReportTime = 0;
unsigned long maxTime = 0;

void debugStartTime(unsigned long &startLoop_time)
{
    startLoop_time = esp_timer_get_time();
}

void debugEndTime(unsigned long &startLoop_time)
{
    unsigned long endLoop = esp_timer_get_time();
    unsigned long elapsedTime = endLoop - startLoop_time;
    accumulatedTime += elapsedTime;
    loopCount++;

    // Check if the elapsed time is the maximum observed
    if (elapsedTime > maxTime)
    {
        maxTime = elapsedTime;
    }

    // Check if one second has passed since the last report
    if ((endLoop - lastReportTime) >= 1000000) // 1 second = 1,000,000 microseconds
    {
        unsigned long averageTime = accumulatedTime / loopCount;

        Serial.print("Average time: ");
        Serial.print(averageTime);
        Serial.println(" us");

        Serial.print("Max time: ");
        Serial.print(maxTime);
        Serial.println(" us");

        // Reset counters for the next interval
        accumulatedTime = 0;
        loopCount = 0;
        lastReportTime = endLoop;
        maxTime = 0; // Reset max time for the next interval
    }
}