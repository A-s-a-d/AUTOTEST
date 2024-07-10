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

void setup()
{
  uart2.init();
  Serial.begin(9600);
  // uart2.write("ESP32 9600 Bauds, 8bits, 1 Stop, Even parity, LSB first, Inverted\n");
  button.begin_interruption();
}

void loop()
{
  test.chose_mode();

  if (button.process_data() /*| test.current_state()*/) // start button pressed
  {
    test.START_TEST_CYCLE();
    Serial.println("SET");
  }
  else // STOP button pressed
  {
    Serial.println("RESET");
    // Stop or reset test state
    // Reset variables
    // Reset positions of actuators
    // Reset position etc
  }
  delay(1000);
}