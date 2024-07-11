#include "BUTTON.h"

volatile boolean START_ISR = false;
volatile boolean STOP_ISR = false; // Initially true because it's normally closed (HIGH)

volatile boolean start = false;
volatile boolean stop = false;

BUTTON::BUTTON(/* args */)
{
}

void BUTTON::begin_interruption()
{
    pinMode(PIN_START, INPUT_PULLDOWN);
    pinMode(PIN_STOP, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_START), START_pressed_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_STOP), STOP_pressed_ISR, FALLING);
}

void IRAM_ATTR BUTTON::START_pressed_ISR()
{
    START_ISR = 1;
}

void IRAM_ATTR BUTTON::STOP_pressed_ISR()
{
    STOP_ISR = 1;
}

void BUTTON::update()
{
    // Disable interrupts while accessing shared variables to prevent race conditions
    noInterrupts();
    start = START_ISR;
    stop = STOP_ISR;
    START_ISR = 0;
    STOP_ISR = 0;
    interrupts();
}
