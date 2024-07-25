#include "main.h"
#include "DRIVER.h"

BUTTON button;
TEST test;

UART_2 uart2;
boolean reset = 1;
CARTE_AFFICHEUR_H2_AYAC100811_2_I2C display;
DRIVER driver;

void setup()
{
  setCpuFrequencyMhz(240);
  uart2.init();
  Serial.begin(250000);
  button.begin_interruption();
  Wire.begin();

  driver.begin();
  driver.release(0);
  delay(5);
  driver.move_bwd(0);
  delay(3000);
  driver.release(0);

  display.begin();
  display.setcursor(0, 0);

  display.clear();
}

void loop()
{
#define TEST_ALLOW 1
#if TEST_ALLOW
#define MAIN_ALLOW 0
#else
#define MAIN_ALLOW 1
#endif

#if debug_loop_timing && debug
  unsigned long Loop_time;
  debugStartTime(Loop_time);
#endif
// !  ******************************************** TEST PROGRAM HERE **********************************************
#if TEST_ALLOW
#define timemov 500
#define timestall 2000

  driver.move_fwd(0);
  delay(timemov);
  driver.release(0);
  delay(timestall);
  driver.move_bwd(0);
  delay(timemov);
  driver.release(0);
  delay(timestall);

#endif

  // !  ******************************************** TEST PROGRAM HERE **********************************************

  //* ******************************************** MAIN CODE ******************************************************
#if MAIN_ALLOW

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

#endif
  //* ******************************************** MAIN CODE ******************************************************

#if debug_loop_timing & debug
  debugEndTime(Loop_time);
#endif
}
