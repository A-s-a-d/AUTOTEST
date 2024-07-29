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
 * also @see https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/stacking-shields for how to stack the Driver sheilds
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

#define DC 0
#define STEPPER 1
#define MOTOR_TYPE DC
#define un_used_port 0XFF

typedef enum BUTTON_INDEX
{
    INDEX_PRIM_DUO_ON_OFF,
    INDEX_PRIM_DUO_SET,
    INDEX_PRIM_DUO_CAL,
    INDEX_PROPILOT_MENU_ESC,
    INDEX_PROPILOT_SET,
    INDEX_PROPILOT_UP,
    INDEX_PROPILOT_DOWN,
    INDEX_PROPILOT_CAL_OK,
    INDEX_DUO_ON_OFF_2,
    INDEX_DUO_SET_2,
    INDEX_DUO_CAL_2
} BUTTON_INDEX;

// Define states for the state machines
enum BUTTON_STATE
{

    POS_0_IDLE,

    MOVE_FWD_CLOSE,
    MOVE_CLOSE_DONE,

    READY_TO_PRESS,

    PRESS_BUTTON_SLOW,
    PRESSED,

    BUTTON_UNPRESS,
    WAIT_UNPRESS,

    UNPRESSED, // return to READY_to_PRESS after this unless otherwise needed

    RETURN_POS_0
};

typedef struct MoveTask
{
    uint8_t current_state;
    uint8_t button;

    unsigned long startTime_move_fwd_fast;

    unsigned long startTime_press = 0;
    unsigned long release_time = 0;
    unsigned long time_unpress = 0;

} MoveTask;

// Drivers present or not for DC configuration
typedef struct DRIVER_PRESET
{
    boolean AFMS_0_Present;
    boolean AFMS_1_Present;
    boolean AFMS_2_Present;
} DRIVER_PRESENT;
#if MOTOR_TYPE == DC
#define DRIVER_0 0x60
#define DRIVER_1 0x61
#define DRIVER_2 0x62

#define PORT_PRIM_DUO_ON_OFF 1
#define PORT_PRIM_DUO_SET 2
#define PORT_PRIM_DUO_CAL 3
#define PORT_PROPILOT_MENU_ESC 4

#define PORT_PROPILOT_SET 1
#define PORT_PROPILOT_UP 2
#define PORT_PROPILOT_DOWN 3
#define PORT_PROPILOT_CAL_OK 4

#define PORT_DUO_ON_OFF_2 1
#define PORT_DUO_SET_2 2
#define PORT_DUO_CAL_2 3

//! We need 20% max duty cycle for PQ12-S model of accutator so 20% of 255 is 51.
const uint8_t speed = 20; // @param  speed The 8-bit PWM value, 0 is off, 255 is on

//  speed The 16-bit/12bit-real PWM value, 0 is off, 4095 is on full
//! We need 20% max duty cycle for PQ12-S model of accutator so 20% of 4095 is 819.
const uint16_t speed_fine = 800;

#elif MOTOR_TYPE == STEPPER
// TODO if STEPPER MOTOR IS USED
#define DRIVER_0 0x60
#define DRIVER_1 0x61
#define DRIVER_2 0x62
#define DRIVER_3 0x63
#define DRIVER_4 0x64
#define DRIVER_5 0x65

#define PORT_PRIM_DUO_ON_OFF 1
#define PORT_PRIM_DUO_SET 2
#define PORT_PRIM_DUO_CAL 1

#define PORT_PROPILOT_MENU_ESC 2
#define PORT_PROPILOT_SET 1
#define PORT_PROPILOT_UP 2
#define PORT_PROPILOT_DOWN 1
#define PORT_PROPILOT_CAL_OK 2

#define PORT_DUO_ON_OFF_2 1
#define PORT_DUO_SET_2 2
#define PORT_DUO_CAL_2 1
uint16_t number_of_steps_16bit = 65535; // verify the that we can put here.
uint16_t round_per_minute = 65535;      // max 65535

#endif

class DRIVER
{
private:
#if MOTOR_TYPE == DC
    // Motor shield instances
    Adafruit_MotorShield AFMS_0;
    Adafruit_MotorShield AFMS_1;
    Adafruit_MotorShield AFMS_2;

    Adafruit_DCMotor *MOTOR_PRIM_DUO_ON_OFF;
    Adafruit_DCMotor *MOTOR_PRIM_DUO_SET;
    Adafruit_DCMotor *MOTOR_PRIM_DUO_CAL;
    Adafruit_DCMotor *MOTOR_PROPILOT_MENU_ESC;
    Adafruit_DCMotor *MOTOR_PROPILOT_SET;
    Adafruit_DCMotor *MOTOR_PROPILOT_UP;
    Adafruit_DCMotor *MOTOR_PROPILOT_DOWN;
    Adafruit_DCMotor *MOTOR_PROPILOT_CAL_OK;
    Adafruit_DCMotor *MOTOR_DUO_ON_OFF_2;
    Adafruit_DCMotor *MOTOR_DUO_SET_2;
    Adafruit_DCMotor *MOTOR_DUO_CAL_2;

#elif MOTOR_TYPE == STEPPER

    Adafruit_MotorShield AFMS_0;
    Adafruit_MotorShield AFMS_1;
    Adafruit_MotorShield AFMS_2;
    Adafruit_MotorShield AFMS_3;
    Adafruit_MotorShield AFMS_4;
    Adafruit_MotorShield AFMS_5;

    Adafruit_StepperMotor *MOTOR_PRIM_DUO_ON_OFF;
    Adafruit_StepperMotor *MOTOR_PRIM_DUO_SET;
    Adafruit_StepperMotor *MOTOR_PRIM_DUO_CAL;
    Adafruit_StepperMotor *MOTOR_PROPILOT_MENU_ESC;
    Adafruit_StepperMotor *MOTOR_PROPILOT_SET;
    Adafruit_StepperMotor *MOTOR_PROPILOT_UP;
    Adafruit_StepperMotor *MOTOR_PROPILOT_DOWN;
    Adafruit_StepperMotor *MOTOR_PROPILOT_CAL_OK;
    Adafruit_StepperMotor *MOTOR_DUO_ON_OFF_2;
    Adafruit_StepperMotor *MOTOR_DUO_SET_2;
    Adafruit_StepperMotor *MOTOR_DUO_CAL_2;

#endif

    uint8_t _current_speed_percent;
    // Global variables to hold the state and timing information
    MoveTask task_BUTTON[11];
    DRIVER_PRESET driver_present;

public:
    DRIVER();
    boolean scan_for_driver(uint8_t adress);
    void begin();

#if MOTOR_TYPE == DC

    void set_speed(uint8_t PWM_duty_cycle_percent);
    void move_fwd(uint8_t button_index);
    void move_bwd(uint8_t button_index);
    void release(uint8_t button_index);
    void reset_position();

    /**
     * @brief
     *
     * @param BUTTON
     * @attention needs to be called peridically. maybe inside main loop and needs to be unblocked and also for all of the buttons or atleast those in use by using for(int i = 0; i<=maxbuttons; i++){ACUTATOR_CYCLE(i);}
     */
    void ACUTATOR_cycle(uint8_t button);
    void move_close(uint8_t button);
    void press_button(uint8_t button);

#elif MOTOR_TYPE == STEPPER
    /**
     * @brief
     *
     * @param steps is how many steps you'd like it to take.
     * @param direction is either FORWARD or BACKWARD
     * @param steptype step type is SINGLE, DOUBLE, INTERLEAVE or MICROSTEP. \n
     *  steptype->`"Single" `means single-coil activation;
     *
     *  steptype->`"Double"` means 2 coils are activated at once (for higher torque)  \n
     *
     *  steptype->`"Interleave"` means that it alternates between single and double to get twice the resolution (but of course its half the speed).  \n
     *
     *  steptype->`"Microstepping"` is a method where the coils are PWM'd to create smooth motion between steps.
     */
    void step(uint16_t steps, uint8_t direction, uint8_t steptype);
    void move_fwd(uint16_t steps_speed);
    void move_bwd(uint16_t steps_speed);

#endif

    void move_closer();
    void press_ONOFF();
    void press_SET();
    void press_CAL();
};

#endif // DRIVER_H