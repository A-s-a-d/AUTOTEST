#ifndef AUTOTEST_CYCLE_H
#define AUTOTEST_CYCLE_H

#include <Arduino.h>
#include "UART.h"
#include "ACUTATOR.h"

#define PH7 "7"
#define PH10 "10"
#define PH_HIZ "H1"

class TEST
{
private:
    struct STATE
    {
        enum Num
        {
            MOV_ACUTATORS,
            PH_7,
            press_ON_OFF_BUTTON,
            press_SET_BUTTON,
            PH_10,
            HIZ,
            END_CYCLE
        };
        Num currentState;
    } STATE;

    struct MODE
    {
        uint8_t MODE = 0;
        const uint8_t AP1 = 0;
        const uint8_t BP1 = 1;
        const uint8_t AR1 = 2;
        const uint8_t AR3 = 3;
    } MODE;

public:
    TEST(/* args */);
    void chose_mode();
    void START_TEST_CYCLE();
    boolean current_state();
};

extern UART_2 uart2;
extern ACUTATOR acutator;

#endif