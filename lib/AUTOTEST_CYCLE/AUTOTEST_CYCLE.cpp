#include "AUTOTEST_CYCLE.h"

#define wait(x) delay(x)

TEST::TEST(/* args */)
{
}

void TEST::chose_mode()
{
}

void TEST::START_TEST_CYCLE()
{
    switch (STATE.currentState)
    {
    case STATE::MOV_ACUTATORS:
        acutator.move_closer();
        wait(1000);
        STATE.currentState = STATE::PH_7;
        break;

    case STATE::PH_7:
        //  code for PH_7 state
        /* Simulator in pH 7 mode
           Wait 100ms or just a little.
           Verify PH value by asking the Simulator
        */
        uart2.write(PH7);
        wait(2000);
        STATE.currentState = STATE::press_ON_OFF_BUTTON;

        break;

    case STATE::press_ON_OFF_BUTTON:
        //  code for press_ON_OFF_BUTTON state
        STATE.currentState = STATE::press_SET_BUTTON;

        break;

    case STATE::press_SET_BUTTON:
        //  code for press_SET_BUTTON state
        STATE.currentState = STATE::PH_10;

        break;

    case STATE::PH_10:
        //  code for PH_10 state
        uart2.write(PH10);
        wait(2000);
        STATE.currentState = STATE::HIZ;

        break;

    case STATE::HIZ:
        //  code for HIZ state
        uart2.write(PH_HIZ);
        wait(2000);
        STATE.currentState = STATE::END_CYCLE;

        break;

    default:
        break;
    }
}
boolean TEST::current_state()
{
    return static_cast<bool>(STATE.currentState);
}
