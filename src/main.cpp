#include "main.h"

// !   millis can be replaced by : esp_timer_get_time();

BUTTON button;
TEST test;

UART_2 uart2;
boolean reset = 1;
CARTE_AFFICHEUR_H2_AYAC100811_2_I2C display;

void setup()
{
  //* Init uC configuration
  setCpuFrequencyMhz(240);      //  Init clock uC
  Wire.begin(SDA, SCL, 100000); // Init I2C
  delayMicroseconds(1);
  uart2.init(); // Init UART2
  delayMicroseconds(1);
  Serial.begin(921600); // Init UART SERIAL MONITOR

  //* Init driver and actuator
  test.begin(); // initialisation de driver et acctionneur

  display.begin();
  display.setcursor(0, 0);
  display.clear();
  display.print("123");
  Serial.print("setup");

  delayMicroseconds(1);
  button.begin_interruption();
  delayMicroseconds(1);
}

void loop()
{

#define TEST_ALLOW 0
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

#endif

  // !  ******************************************** TEST PROGRAM HERE **********************************************

  // * ******************************************** MAIN CODE ******************************************************
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
  test.update_cycle();

#endif
  // * ******************************************** MAIN CODE ******************************************************

#if debug_loop_timing & debug
  debugEndTime(Loop_time);
#endif
}
