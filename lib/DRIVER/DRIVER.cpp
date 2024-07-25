
#include "DRIVER.h"

DRIVER::DRIVER() :
#if MOTOR_TYPE == DC
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2)
#elif MOTOR_TYPE == STEPPER
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2), AFMS_3(DRIVER_3), AFMS_4(DRIVER_4), AFMS_5(DRIVER_5)
#endif
{
}

void DRIVER::begin()
{

#if MOTOR_TYPE == DC

    MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getMotor(PRIM_DUO_ON_OFF);
    MOTOR_PRIM_DUO_SET = AFMS_0.getMotor(PRIM_DUO_SET);
    MOTOR_PRIM_DUO_CAL = AFMS_0.getMotor(PRIM_DUO_CAL);
    MOTOR_PROPILOT_MENU_ESC = AFMS_0.getMotor(PROPILOT_MENU_ESC);

    MOTOR_PROPILOT_SET = AFMS_1.getMotor(PROPILOT_SET);
    MOTOR_PROPILOT_UP = AFMS_1.getMotor(PROPILOT_UP);
    MOTOR_PROPILOT_DOWN = AFMS_1.getMotor(PROPILOT_DOWN);
    MOTOR_PROPILOT_CAL_OK = AFMS_1.getMotor(PROPILOT_CAL_OK);

    MOTOR_DUO_ON_OFF_2 = AFMS_2.getMotor(DUO_ON_OFF_2);
    MOTOR_DUO_SET_2 = AFMS_2.getMotor(DUO_SET_2);
    MOTOR_DUO_CAL_2 = AFMS_2.getMotor(DUO_CAL_2);

    AFMS_0.begin();
    AFMS_1.begin();
    AFMS_2.begin();

    MOTOR_PRIM_DUO_ON_OFF->setSpeed(speed);
    MOTOR_PRIM_DUO_SET->setSpeed(speed);
    MOTOR_PRIM_DUO_CAL->setSpeed(speed);
    MOTOR_PROPILOT_MENU_ESC->setSpeed(speed);
    MOTOR_PROPILOT_SET->setSpeed(speed);
    MOTOR_PROPILOT_UP->setSpeed(speed);
    MOTOR_PROPILOT_DOWN->setSpeed(speed);
    MOTOR_PROPILOT_CAL_OK->setSpeed(speed);
    MOTOR_DUO_ON_OFF_2->setSpeed(speed);
    MOTOR_DUO_SET_2->setSpeed(speed);
    MOTOR_DUO_CAL_2->setSpeed(speed);
    // for 12 bit speed control more prescise : setSpeedFine(uint16_t speed)

#elif MOTOR_TYPE == STEPPER
    MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getStepper(number_of_steps_16bit, PRIM_DUO_ON_OFF);
    MOTOR_PRIM_DUO_SET = AFMS_0.getStepper(number_of_steps_16bit, PRIM_DUO_SET);
    MOTOR_PRIM_DUO_CAL = AFMS_1.getStepper(number_of_steps_16bit, PRIM_DUO_CAL);
    MOTOR_PROPILOT_MENU_ESC = AFMS_1.getStepper(number_of_steps_16bit, PROPILOT_MENU_ESC);
    MOTOR_PROPILOT_SET = AFMS_2.getStepper(number_of_steps_16bit, PROPILOT_SET);
    MOTOR_PROPILOT_UP = AFMS_2.getStepper(number_of_steps_16bit, PROPILOT_UP);
    MOTOR_PROPILOT_DOWN = AFMS_3.getStepper(number_of_steps_16bit, PROPILOT_DOWN);
    MOTOR_PROPILOT_CAL_OK = AFMS_3.getStepper(number_of_steps_16bit, PROPILOT_CAL_OK);
    MOTOR_DUO_ON_OFF_2 = AFMS_4.getStepper(number_of_steps_16bit, DUO_ON_OFF_2);
    MOTOR_DUO_SET_2 = AFMS_4.getStepper(number_of_steps_16bit, DUO_SET_2);
    MOTOR_DUO_CAL_2 = AFMS_5.getStepper(number_of_steps_16bit, DUO_CAL_2);

    AFMS_0.begin();
    AFMS_1.begin();
    AFMS_2.begin();
    AFMS_3.begin();
    AFMS_4.begin();
    AFMS_5.begin();

    MOTOR_PRIM_DUO_ON_OFF->setSpeed(round_per_minute);
    MOTOR_PRIM_DUO_SET->setSpeed(round_per_minute);
    MOTOR_PRIM_DUO_CAL->setSpeed(round_per_minute);
    MOTOR_PROPILOT_MENU_ESC->setSpeed(round_per_minute);
    MOTOR_PROPILOT_SET->setSpeed(round_per_minute);
    MOTOR_PROPILOT_UP->setSpeed(round_per_minute);
    MOTOR_PROPILOT_DOWN->setSpeed(round_per_minute);
    MOTOR_PROPILOT_CAL_OK->setSpeed(round_per_minute);
    MOTOR_DUO_ON_OFF_2->setSpeed(round_per_minute);
    MOTOR_DUO_SET_2->setSpeed(round_per_minute);
    MOTOR_DUO_CAL_2->setSpeed(round_per_minute);
#endif
}

void DRIVER::step(uint16_t steps, uint8_t direction, uint8_t steptype)
{
#if MOTOR_TYPE == DC

#elif MOTOR_TYPE == STEPPER
    MOTOR_DUO_CAL_2->step(steps, direction, steptype);
#endif
}
void DRIVER::move_fwd_single(uint16_t steps)
{

#if MOTOR_TYPE == DC

#elif MOTOR_TYPE == STEPPER
    MOTOR_DUO_CAL_2->step(number_of_steps_16bit, FORWARD, SINGLE);

#endif

    // Implementation for moving forward
}

void DRIVER::move_bwd_single(uint16_t steps)
{
#if MOTOR_TYPE == DC

#elif MOTOR_TYPE == STEPPER
    MOTOR_DUO_CAL_2->step(number_of_steps_16bit, BACKWARD, SINGLE);

#endif
    // Implementation for moving backward
}

void DRIVER::move_closer()
{
    // Implementation for moving closer
}
