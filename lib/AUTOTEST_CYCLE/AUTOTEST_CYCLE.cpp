#include "AUTOTEST_CYCLE.h"

DRIVER driver;
#define button_cal 1
TEST::TEST(/* args */)
{
}

void TEST::begin()
{
    driver.begin();
}
unsigned long previousMillis = 0;
const long interval = 5000; // interval in milliseconds

void TEST::update_cycle()
{
    driver.ACUTATOR_cycle(button_cal);
    // CYCLE();
}

void TEST::chose_mode()
{
}
#define delay_1s 1000

unsigned long press_cal_time = 0;
unsigned long previous_state_display = 0; // Stores the last time STATE.currentState was printed
const long time_current_state_display = 100;

void TEST::CYCLE()
{

    if (previous_state_display == 0)
    {
        previous_state_display = millis();
    }

    if (millis() - previous_state_display >= time_current_state_display)
    {
        Serial.print("state_test");
        Serial.println(STATE.currentState);
        previous_state_display = 0;
    }

    static unsigned long time = 0;
    static uint8_t time_count = 0;
    unsigned long startTime = millis();

    switch (STATE.currentState)
    {
    case STATE::INIT:
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
    case STATE::WAITING_FOR_CYCLE_TO_START:

        break;

    case STATE::MOV_ACUTATORS:
        driver.move_close(button_cal); // ! just to test the program needs to be removed in the main program.
        while (millis() - startTime < 2000)
        {
            driver.ACUTATOR_cycle(button_cal);
        } // takes 2sec.
        Serial.println("moveclose");
        STATE.currentState = STATE::PH_7;
        break;

    case STATE::PH_7:
        //  code for PH_7 state
        /* Simulator in pH 7 mode
           Wait 100ms or just a little.
           Verify PH value by asking the Simulator
        */
        if (press_cal_time == 0)
        {
            press_cal_time = millis();
        }
        uart2.write("7");
        if (millis() - press_cal_time >= delay_1s)
        {
            press_cal_time = 0;
            Serial.println("press");

            STATE.currentState = STATE::press_ON_OFF_BUTTON;
        }

        break;

    case STATE::press_ON_OFF_BUTTON:
        //  code for press_ON_OFF_BUTTON state
        if (press_cal_time == 0)
        {
            press_cal_time = millis();
            driver.press_button(button_cal);
        }
        if (millis() - press_cal_time >= 2000)
        {
            press_cal_time = 0;
            Serial.println("press");
            STATE.currentState = STATE::press_SET_BUTTON;
        }
        break;

    case STATE::press_SET_BUTTON:
        //  code for press_SET_BUTTON state
        if (press_cal_time == 0)
        {
            press_cal_time = millis();
            driver.press_button(button_cal);
        }

        if (millis() - press_cal_time >= 2000)
        {
            press_cal_time = 0;
            STATE.currentState = STATE::PH_10;
        }
        break;

    case STATE::PH_10:
        //  code for PH_10 state
        static boolean uart_data_sent = 0;
        if (press_cal_time == 0)
        {
            press_cal_time = millis();
            uart2.write("10");
        }
        // if (uart_data_sent == 0)
        // {
        //     if (uart_write_bytes(UART_NUM, "10", strlen("10")) == 2)
        //     {
        //         uart_write_bytes(UART_NUM, CR, strlen(CR));
        //     }
        // }

        if (millis() - press_cal_time >= 2000)
        {
            press_cal_time = 0;
            STATE.currentState = STATE::HIZ;
        }
        break;

    case STATE::HIZ:
        //  code for HIZ state
        if (press_cal_time == 0)
        {
            press_cal_time = millis();
            uart2.write("H1");
        }
        if (millis() - press_cal_time >= 25000)
        {
            press_cal_time = 0;
            STATE.currentState = STATE::END_OF_CYCLE;
        }
        break;

    case STATE::END_OF_CYCLE:
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
        STATE.currentState = STATE::INIT;
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
