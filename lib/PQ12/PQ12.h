#ifndef PQ12_H
#define PQ12_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>

class PQ12
{
private:
    const uint8_t _speed = 100;
    const uint8_t _port = 1;
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();

    Adafruit_DCMotor *myMotor = AFMS.getMotor(_port);

public:
    PQ12(/* args */);
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