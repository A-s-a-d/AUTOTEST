/**
 * @file DRIVER.h
 * @author MUHAMMAD ASAD
 * @brief file for adaptable program to press different buttons.
 * the driver used here is the module Adafruit 2927 FeatherWing.
 *
 * @note This is the DC Motor + Stepper FeatherWing which will let you use :
 * *2 x bi-polar stepper motors
 * or
 * * 4 x brushed DC motors
 *   Using Feather Stacking Headers
 * Since the FeatherWing only uses the I2C (SDA & SCL pins),it works with any and all Feathers.
 * You can stack it with any other FeatherWing or with itself
 * ! (just make sure you have each wing with a unique I2C address)
 * * Motor FeatherWing Specs:
 * 4 full H-Bridges: the @b TB6612 chipset provides 1.2A per bridge with thermal
 * shutdown protection, internal kickback protection diodes.
 * Can run motors on @a 4.5VDC to @a 13.5VDC.
 * Up to 4 bi-directional DC motors with individual 12-bit speed selection (so, about 0.02% resolution).
 * Up to 2 stepper motors (unipolar or bipolar) with single coil, double coil, interleaved or micro-stepping.
 * ! Motors automatically disabled on power-up.
 * * Big 3.5mm terminal block connectors to easily hook up wires (18-26AWG) and power.
 * Polarity protected 2-pin terminal block and jumper to connect external power, for separate logic/motor supplies
 * Completely stackable design:
 * @c 5_address-select-jumper-pads means up to @c 32-stackable-wings: that's @c 64-steppers or @c 128-DC-motors!
 *
 * @see https://www.adafruit.com/product/2927
 *
 * @deprecated description
 * TODO Implement multiple drivers.
 * TODO Associate different ports of each driver to a motor/accutator.
 * TODO Associate each port to each button that needs to be pressed.
 * TODO Press function
 * TODO UNPRESS function
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include "global.h"

#define DC 0;
#define STEPPER 1;
#define MOTOR_TYPE DC;

#if !MOTOR_TYPE
#define get(port) getMotor(port)
typedef struct data
{
    const uint8_t speed = 100;
    const uint8_t port = 1;
} data;
#endif

#if MOTOR_TYPE
#define get(port) getStepper(port)
typedef struct data
{
    const uint16_t steps = 200;
    const uint8_t port = 2;
    const uint8_t rpm = 100;
} data;
#endif

class DRIVER
{
private:
    data _data;

    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    Adafruit_DCMotor *myMotor = AFMS.get(_data.port);

public:
    DRIVER(/* args */);
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