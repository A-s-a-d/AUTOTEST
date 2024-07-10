#include "BUTTON.h"

struct BUTTON_PRESSED
{
    volatile boolean START = 0;
    volatile boolean STOP = 0;
} BUTTON_PRESSED;

BUTTON::BUTTON(/* args */)
{
}

void BUTTON::begin_interruption()
{
    pinMode(PIN_START, INPUT_PULLDOWN);
    pinMode(PIN_STOP, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PIN_START), START_pressed_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_STOP), STOP_pressed_ISR, FALLING);
}

void IRAM_ATTR BUTTON::START_pressed_ISR()
{
    BUTTON_PRESSED.STOP = 0;
    BUTTON_PRESSED.START = 1;
}

void IRAM_ATTR BUTTON::STOP_pressed_ISR()
{
    BUTTON_PRESSED.START = 0;
    BUTTON_PRESSED.STOP = 1;
}

boolean BUTTON::process_data()
{
    // Disable interrupts while accessing shared variables to prevent race conditions
    boolean start, stop, test_instructions;
    noInterrupts();
    start = BUTTON_PRESSED.START;
    stop = BUTTON_PRESSED.STOP;
    interrupts();

    // Perform the logic outside of critical section
    test_instructions = start && !stop;
    return test_instructions;
}
