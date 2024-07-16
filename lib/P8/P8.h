#ifndef P8_H
#define P8_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>

class P8
{
private:
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    const uint16_t _steps = 200;
    const uint8_t _port = 2;
    const uint8_t _rpm = 100;
    Adafruit_StepperMotor *myMotor = AFMS.getStepper(_steps, _port);

public:
    uint8_t steps;
    char *direction; // FORWARD or BACKWARD
    char *steptype;  // SINGLE, DOUBLE, INTERLEAVE or MICROSTEP
                     /*
                     "Single" means single-coil activation
                     "Double" means 2 coils are activated at once (for higher torque)
                     "Interleave" means that it alternates between single and double to get twice the resolution (but of course its half the speed).
                     "Microstepping" is a method where the coils are PWM'd to create smooth motion between steps.*/
    P8(/* args */);
    void begin();
    void move_fwd();
    void move_bwd();
    void dtct_press();
    void dtct_proximity();
    void move_closer();
    void press_ONOFF();
    void press_SET();
    void press_CAL();
};

#endif