#include "main.h"


BUTTON button;
TEST test;

UART_2 uart2;
boolean reset = 1;
CARTE_AFFICHEUR_H2_AYAC100811_2_I2C display;
void scanner();

void setup()
{
  setCpuFrequencyMhz(240);
  uart2.init();
  Serial.begin(250000);
  button.begin_interruption();
  Wire.begin();
  display.begin();
  display.setcursor(0, 0);
}

void loop()
{
#if debug_loop_timing && debug
unsigned long Loop_time;
  debugStartTime(Loop_time);
#endif

  //* ******************************************** MAIN CODE ******************************************************
  button.update();

  if (stop)
  {
    reset = 1;
    Serial.print("stop");
  }
  else if (start)
  {
    reset = 0;
    Serial.print("start");
  }
  stop = 0;
  start = 0;

  if (test.current_state() == test.STATE.WAITING_FOR_CYCLE_TO_START && !reset)
  {
    reset = 0;
    test.start_cycle();
  }
  if (test.ongoing())
  {
    display.test_ongoing(true);
    if (stop)
    {
      reset = 1;
    }
  }
  else
  {
    display.test_ongoing(false);
  }

  if (test.current_state() == test.STATE.END_OF_CYCLE)
  {
    reset = 1;
  }
  test.CYCLE();

  //* ******************************************** MAIN CODE ******************************************************

#if debug_loop_timing & debug
  debugEndTime(Loop_time);
#endif
}
