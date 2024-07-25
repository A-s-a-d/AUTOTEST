
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

    MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getMotor(PORT_PRIM_DUO_ON_OFF);
    MOTOR_PRIM_DUO_SET = AFMS_0.getMotor(PORT_PRIM_DUO_SET);
    MOTOR_PRIM_DUO_CAL = AFMS_0.getMotor(PORT_PRIM_DUO_CAL);
    // MOTOR_PROPILOT_MENU_ESC = AFMS_0.getMotor(PORT_PROPILOT_MENU_ESC);

    // MOTOR_PROPILOT_SET = AFMS_1.getMotor(PORT_PROPILOT_SET);
    // MOTOR_PROPILOT_UP = AFMS_1.getMotor(PORT_PROPILOT_UP);
    // MOTOR_PROPILOT_DOWN = AFMS_1.getMotor(PORT_PROPILOT_DOWN);
    // MOTOR_PROPILOT_CAL_OK = AFMS_1.getMotor(PORT_PROPILOT_CAL_OK);
    // MOTOR_DUO_ON_OFF_2 = AFMS_2.getMotor(PORT_DUO_ON_OFF_2);
    // MOTOR_DUO_SET_2 = AFMS_2.getMotor(PORT_DUO_SET_2);
    // MOTOR_DUO_CAL_2 = AFMS_2.getMotor(PORT_DUO_CAL_2);

    AFMS_0.begin();
    // AFMS_1.begin();
    // AFMS_2.begin();

    set_speed(20 /* % percent*/);

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

#if MOTOR_TYPE == DC

void DRIVER::set_speed(uint8_t PWM_duty_cycle_percent)
{
    uint8_t PWM;
    PWM = (PWM_duty_cycle_percent >= 20) ? 51 : static_cast<uint8_t>((static_cast<float>(PWM_duty_cycle_percent) / 100.0) * 255);
    MOTOR_PRIM_DUO_ON_OFF->setSpeed(PWM);
    MOTOR_PRIM_DUO_SET->setSpeed(PWM);
    MOTOR_PRIM_DUO_CAL->setSpeed(PWM);
    // MOTOR_PROPILOT_MENU_ESC->setSpeed(PWM);
    // MOTOR_PROPILOT_SET->setSpeed(PWM);
    // MOTOR_PROPILOT_UP->setSpeed(PWM);
    // MOTOR_PROPILOT_DOWN->setSpeed(PWM);
    // MOTOR_PROPILOT_CAL_OK->setSpeed(PWM);
    // MOTOR_DUO_ON_OFF_2->setSpeed(PWM);
    // MOTOR_DUO_SET_2->setSpeed(PWM);
    // MOTOR_DUO_CAL_2->setSpeed(PWM);
}

void DRIVER::move_fwd(uint8_t button_index)
{
    uint8_t DIRECTION = FORWARD;

    switch (button_index)
    {
    case BUTTON_INDEX::INDEX_PRIM_DUO_ON_OFF:
        MOTOR_PRIM_DUO_ON_OFF->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_SET:
        MOTOR_PRIM_DUO_SET->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_CAL:
        MOTOR_PRIM_DUO_CAL->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_MENU_ESC:
        MOTOR_PROPILOT_MENU_ESC->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_SET:
        MOTOR_PROPILOT_SET->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_UP:
        MOTOR_PROPILOT_UP->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_DOWN:
        MOTOR_PROPILOT_DOWN->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_CAL_OK:
        MOTOR_PROPILOT_CAL_OK->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_ON_OFF_2:
        MOTOR_DUO_ON_OFF_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_SET_2:
        MOTOR_DUO_SET_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_CAL_2:
        MOTOR_DUO_CAL_2->run(DIRECTION);
        break;
    default:
        break;
    }
}

void DRIVER::move_bwd(uint8_t button_index)
{
    uint8_t DIRECTION = BACKWARD;
    switch (button_index)
    {
    case BUTTON_INDEX::INDEX_PRIM_DUO_ON_OFF:
        MOTOR_PRIM_DUO_ON_OFF->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_SET:
        MOTOR_PRIM_DUO_SET->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_CAL:
        MOTOR_PRIM_DUO_CAL->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_MENU_ESC:
        MOTOR_PROPILOT_MENU_ESC->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_SET:
        MOTOR_PROPILOT_SET->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_UP:
        MOTOR_PROPILOT_UP->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_DOWN:
        MOTOR_PROPILOT_DOWN->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_CAL_OK:
        MOTOR_PROPILOT_CAL_OK->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_ON_OFF_2:
        MOTOR_DUO_ON_OFF_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_SET_2:
        MOTOR_DUO_SET_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_CAL_2:
        MOTOR_DUO_CAL_2->run(DIRECTION);
        break;
    default:
        break;
    }
}

void DRIVER::release(uint8_t button_index)
{
     uint8_t DIRECTION = RELEASE;
    switch (button_index)
    {
    case BUTTON_INDEX::INDEX_PRIM_DUO_ON_OFF:
        MOTOR_PRIM_DUO_ON_OFF->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_SET:
        MOTOR_PRIM_DUO_SET->run(DIRECTION);
        break;

    case BUTTON_INDEX::INDEX_PRIM_DUO_CAL:
        MOTOR_PRIM_DUO_CAL->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_MENU_ESC:
        MOTOR_PROPILOT_MENU_ESC->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_SET:
        MOTOR_PROPILOT_SET->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_UP:
        MOTOR_PROPILOT_UP->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_DOWN:
        MOTOR_PROPILOT_DOWN->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_PROPILOT_CAL_OK:
        MOTOR_PROPILOT_CAL_OK->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_ON_OFF_2:
        MOTOR_DUO_ON_OFF_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_SET_2:
        MOTOR_DUO_SET_2->run(DIRECTION);
        break;
    case BUTTON_INDEX::INDEX_DUO_CAL_2:
        MOTOR_DUO_CAL_2->run(DIRECTION);
        break;
    default:
        break;
    }

}
#elif MOTOR_TYPE == STEPPER

void DRIVER::move_fwd(uint16_t steps_speed)
{

    MOTOR_PRIM_DUO_ON_OFF->run(FORWARD);
    MOTOR_PRIM_DUO_SET->run(FORWARD);
    MOTOR_PRIM_DUO_CAL->run(FORWARD);
    // Implementation for moving forward
}

void DRIVER::move_bwd(uint16_t steps_speed)
{
    MOTOR_DUO_CAL_2->step(number_of_steps_16bit, FORWARD, SINGLE);
}

#endif
void DRIVER::move_closer()
{
    // Implementation for moving closer
}
