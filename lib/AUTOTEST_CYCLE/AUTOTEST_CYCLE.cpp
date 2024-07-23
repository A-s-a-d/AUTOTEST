#include "AUTOTEST_CYCLE.h"

#define wait(x) delay(x)

TEST::TEST(/* args */)
{
}

void TEST::chose_mode()
{
}

void TEST::CYCLE()
{

    switch (STATE.currentState)
    {
    case STATE::WAITING_FOR_CYCLE_TO_START:

        static unsigned long time = 0;
        static uint8_t time_count = 0;

        if (millis() - time >= 100)
        {
            time = millis();
            uart2.write("7");
            time_count++;
        }
        if (time_count >= 2)
        {
            uart2.write("H0");
            time_count = 0; // Reset time_count for the next round
        }
        break;

    case STATE::MOV_ACUTATORS:
        // acutator.move_closer
        wait(1000);
        STATE.currentState = STATE::PH_7;
        break;

    case STATE::PH_7:
        //  code for PH_7 state
        /* Simulator in pH 7 mode
           Wait 100ms or just a little.
           Verify PH value by asking the Simulator
        */
        uart2.write("7");
        wait(2000);
        STATE.currentState = STATE::press_ON_OFF_BUTTON;
        break;

    case STATE::press_ON_OFF_BUTTON:
        //  code for press_ON_OFF_BUTTON state
        STATE.currentState = STATE::press_SET_BUTTON;
        wait(3000);
        break;

    case STATE::press_SET_BUTTON:
        //  code for press_SET_BUTTON state
        STATE.currentState = STATE::PH_10;
        wait(3000);
        break;

    case STATE::PH_10:
        //  code for PH_10 state
        uart2.write(PH10);
        wait(2000);
        STATE.currentState = STATE::HIZ;
        break;

    case STATE::HIZ:
        //  code for HIZ state
        uart2.write("H1");
        wait(19000);
        STATE.currentState = STATE::END_OF_CYCLE;
        break;

    case STATE::END_OF_CYCLE:
        // Serial.print("END");
        // wait(500);
        // uart2.write("7");
        // wait(100);
        // uart2.write("H0");

        static unsigned long time_END = 0;
        static uint8_t time_END_count = 0;

        if (millis() - time >= 100)
        {
            time = millis();
            uart2.write("7");
            time_count++;
        }
        if (time_count >= 2)
        {
            uart2.write("H0");
            time_count = 0; // Reset time_count for the next round
        }
        STATE.currentState = STATE::WAITING_FOR_CYCLE_TO_START;
        break;

    default:
        break;
    }
}

void TEST::start_cycle()
{
    STATE.currentState = STATE.MOV_ACUTATORS;
}

void TEST::end_cycle()
{
    STATE.currentState = STATE.END_OF_CYCLE;
}

uint8_t TEST::current_state()
{
    return STATE.currentState;
}

bool TEST::ongoing()
{
    uint8_t status = current_state();
    if (status != STATE.WAITING_FOR_CYCLE_TO_START && status != STATE.END_OF_CYCLE)
    {
        return true;
    }
    return false;
}
