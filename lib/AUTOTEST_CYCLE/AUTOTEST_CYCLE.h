#ifndef AUTOTEST_CYCLE_H
#define AUTOTEST_CYCLE_H

#include <Arduino.h>
#include "UART.h"
#include "P8.h"
#include "PQ12.h"

#define PH_HIZ "H1"

class TEST
{
private:
public:
    struct STATE
    {
        enum Num
        {
            WAITING_FOR_CYCLE_TO_START,
            MOV_ACUTATORS,
            PH_7,
            press_ON_OFF_BUTTON,
            press_SET_BUTTON,
            PH_10,
            HIZ,
            END_OF_CYCLE
        };
        Num currentState;
    } STATE;

    const char *PH7 = "7";
    const char *PH10 = "10";

    struct MODE
    {
        uint8_t MODE = 0;
        const uint8_t AP1 = 0;
        const uint8_t BP1 = 1;
        const uint8_t AR1 = 2;
        const uint8_t AR3 = 3;
    } MODE;
    TEST(/* args */);
    void chose_mode();
    void CYCLE();
    void start_cycle();
    void end_cycle();
    uint8_t current_state();
    bool ongoing();
};

extern UART_2 uart2;

#endif