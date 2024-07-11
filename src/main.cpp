#include "main.h"
#include <Arduino.h>
#include "SCREEN.h"
#include "BUTTON.h"
#include "TIMER_MILLIS.h"
#include "AUTOTEST_CYCLE.h"
#include "UART.h"
#include "ACUTATOR.h"

BUTTON button;
TEST test;
ACUTATOR acutator;
UART_2 uart2;
boolean reset = 1;

void setup()
{
  uart2.init();
  Serial.begin(9600);
  // uart2.write("ESP32 9600 Bauds, 8bits, 1 Stop, Even parity, LSB first, Inverted\n");
  button.begin_interruption();
}

void loop()
{
  // test.chose_mode();

  button.update();

  if (stop) // start button pressed
  {
    Serial.println("STOP");
    reset = 1;
  }
  else if (start) // STOP button pressed
  {
    Serial.println("START");

    reset = 0;
  }
  stop = 0;
  start = 0;

  if (reset)
  {
    Serial.println("reset");
    reset = 1;
    // reset
  }
  else if (!reset)
  {
    Serial.println("test");

    if (test.current_state() == test.STATE.END_OF_CYCLE)
    {
      reset = 1;
    }
    if (test.current_state() == test.STATE.WAITING_FOR_CYCLE_TO_START)
    {
      reset = 0;
      test.start_test_cycle();
    }
    Serial.println("SET");
    test.TEST_CYCLE();
  }

  delay(1000);
}