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
#define MOTOR_TYPE DC // Define motor type DC = 0; STEPPER = 1
#define un_used_port 0XFF

// Index buttons
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

    POS_0_IDLE, // Actuator at init position

    MOVE_FWD_CLOSE,  // Move Actuator close to button
    MOVE_CLOSE_DONE, // Actuator is close to button (release function) and set slower speed (setspeed(50 /* % */))

    READY_TO_PRESS, // Waiting to receive press command

    PRESS_BUTTON_SLOW, // Move actuator forward to press button, Actuator is pressing button.
    PRESSED,           // Bbutton has been pressed so release(idx) to stop pressing

    BUTTON_UNPRESS, // start moving bwd to return to ready_to_press position
    WAIT_UNPRESS,   // wait for accutator to return to ready_to_press position

    UNPRESSED, // the actuator has returned to READY_TO_PRESS position

    /**
     * @brief
     * TODO Reset the acutators to position 0, we can go to this condition to reset the positions of actuators.
     *  `reset_position()` that is already written can be called to do this.
     */
    RETURN_POS_0
};

typedef struct MoveTask // a struct that will keep trace of different parameters of buttons
{
    uint8_t current_state; // stores the current state of button
    uint8_t button;        // stores the index of button

    unsigned long startTime_move_fwd_fast; // variable to store the time for moving closer to the buttons.  : MOVE_FWD_CLOSE

    unsigned long startTime_press = 0; // variable to store the time for pressing the buttons. : PRESS_BUTTON_SLOW
    unsigned long releaseTime = 0;     // variable to store the time for release of the buttons. : PRESSED
    unsigned long unpressTime = 0;     // variable to store the time forun pressiing of the buttons. : BUTTON_UNPRESS, WAIT_UNPRESS

} MoveTask;

// Drivers (AdaFruit Motor Shield) present or not for DC configuration
typedef struct DRIVER_PRESENT
{
    boolean AFMS_0_Present;
    boolean AFMS_1_Present;
    boolean AFMS_2_Present;
} DRIVER_PRESENT;

#if MOTOR_TYPE == DC
// 4 actuators per driver -> up to 12 actuators

#define DRIVER_0 0x60 // I2C adress of driver 0
#define DRIVER_1 0x61 // I2C adress of driver 1
#define DRIVER_2 0x62 // I2C adress of driver 2

// port numbers of driver 0 associated with button
#define PORT_PRIM_DUO_ON_OFF 1
#define PORT_PRIM_DUO_SET 2
#define PORT_PRIM_DUO_CAL 3
#define PORT_PROPILOT_MENU_ESC 4

// port numbers of driver 1 associated with button
#define PORT_PROPILOT_SET 1
#define PORT_PROPILOT_UP 2
#define PORT_PROPILOT_DOWN 3
#define PORT_PROPILOT_CAL_OK 4

// port numbers of driver 2 associated with button
#define PORT_DUO_ON_OFF_2 1
#define PORT_DUO_SET_2 2
#define PORT_DUO_CAL_2 3

#elif MOTOR_TYPE == STEPPER
// 2 actuators per driver -> up to 12 actuators
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

    // Motor driver shield/module instances
    Adafruit_MotorShield AFMS_0;
    Adafruit_MotorShield AFMS_1;
    Adafruit_MotorShield AFMS_2;

    // Motor driver port/acutators/buttons instances
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
    // Do not use this class, not compatible cause of delays
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

    uint8_t _current_speed_percent; // current speed of DC MOTOR

    MoveTask task_BUTTON[11];      // private struct table to hold the state and timing information for each button, index_button enum can be used between [idx] to acess specific button.
    DRIVER_PRESENT driver_present; // instance of struct to store the presence of driver.

public:
    DRIVER(); // constructor

    /**
     * @brief  scan for given adresse
     *
     * @param adress adress to scan
     * @return boolean returns true if driver present else false
     */
    boolean scan_for_driver(uint8_t adress);

    void begin(); // initialisation of driver according to the configuration and drivers presence

#if MOTOR_TYPE == DC

    /**
     * @brief sets the speed of the motors in percent or in PWM
     * @param PWM_duty_cycle_percent between 0 and 100% if 0 no speed and if hundred max speed.
     */
    void set_speed(uint8_t PWM_duty_cycle_percent);

    /**
     * @brief sends move foward command to a the acutator
     * @param button_index to start moving acutator associated with it. BUTTON_INDEX enum needs to/ can be used here.
     */
    void move_fwd(uint8_t button_index);

    /**
     * @brief sends move backward command to a button
     * @param button_index to start moving acutator associated with it. BUTTON_INDEX enum needs to/ can be used here.
     */
    void move_bwd(uint8_t button_index);

    /**
     * @brief relese the signal/ signal is 0V / 0% PWM.
     * @param button_index
     */
    void release(uint8_t button_index);

    /**
     * @brief resets the position to 0 of all acutators.
     *@attention it's a `blocking` funtion and uses the `delay`, means you can't move on until you have finished the reset so `call this function with care`.
     */
    void reset_position();

    /**
     * @brief the main cycle of ACUTATOR
     *
     * TODO : call this function in `main()` for all of the buttons, ot atleast depending on the device selected, only the buttons that are being used.
     * !  inside main loop and needs to be unblocked and also for all of the buttons or atleast those in use by using :
     * ```for(int i = 0; i<= numbre_of_buttons_used; i++){ACUTATOR_CYCLE(i);} ```
     * @attention needs to be called peridically.

     *  @param BUTTON the button whose cycle needs to be updated
     */
    void ACUTATOR_cycle(uint8_t button);

    /**
     * @attention Can only be called if button is in position 0 and idle, after calling this function the cycle will wait at ready to press condition for the command to press.
     * @brief By providing the index of button it will move the associated  closer to the button without pressing.
     *
     * @param button to move close.
     */
    void move_close(uint8_t button);

    /**
     * @attention Can only be called if button is ready to press.
     * @brief By providing the index of button it will press the button
     *
     * @param button to press.
     */
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
};

#endif // DRIVER_H