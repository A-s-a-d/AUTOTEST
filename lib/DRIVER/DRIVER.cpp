#include "DRIVER.h"

DRIVER::DRIVER() :
#if MOTOR_TYPE == DC
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2)
#elif MOTOR_TYPE == STEPPER
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2), AFMS_3(DRIVER_3), AFMS_4(DRIVER_4), AFMS_5(DRIVER_5)
#endif
{
    for (uint8_t i = 0; i < 11; i++)
    {
        task_BUTTON[i].button = i;
        task_BUTTON[i].current_state = 0;
    }
    _current_speed_percent = 0;
}

boolean DRIVER::scan_for_driver(uint8_t adress)
{
    byte error;
    Wire.beginTransmission(adress);
    error = Wire.endTransmission();
    if (error == 0)
    {
        return true;
    }
    return false;
}

void DRIVER::begin()
{
#if MOTOR_TYPE == DC
#define tries 5

    for (uint8_t i = 0; !driver_present.AFMS_0_Present && (i <= tries); i++)
    {
        MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getMotor(PORT_PRIM_DUO_ON_OFF);
        MOTOR_PRIM_DUO_SET = AFMS_0.getMotor(PORT_PRIM_DUO_SET);
        MOTOR_PRIM_DUO_CAL = AFMS_0.getMotor(PORT_PRIM_DUO_CAL);
        MOTOR_PROPILOT_MENU_ESC = AFMS_0.getMotor(PORT_PROPILOT_MENU_ESC);
        if (scan_for_driver(DRIVER_0) == true)
        {
            driver_present.AFMS_0_Present = true;
            AFMS_0.begin();
        }
        else
        {
            driver_present.AFMS_0_Present = false;
        }
    }

    for (uint8_t i = 0; !driver_present.AFMS_1_Present && (i < tries); ++i)
    {
        MOTOR_PROPILOT_SET = AFMS_1.getMotor(PORT_PROPILOT_SET);
        MOTOR_PROPILOT_UP = AFMS_1.getMotor(PORT_PROPILOT_UP);
        MOTOR_PROPILOT_DOWN = AFMS_1.getMotor(PORT_PROPILOT_DOWN);
        MOTOR_PROPILOT_CAL_OK = AFMS_1.getMotor(PORT_PROPILOT_CAL_OK);
        if (scan_for_driver(DRIVER_1) == true)
        {
            driver_present.AFMS_1_Present = true;
            AFMS_1.begin();
        }
        else
        {
            driver_present.AFMS_1_Present = false;
        }
    }

    for (uint8_t i = 0; !driver_present.AFMS_2_Present && (i < tries); ++i)
    {
        MOTOR_DUO_ON_OFF_2 = AFMS_2.getMotor(PORT_DUO_ON_OFF_2);
        MOTOR_DUO_SET_2 = AFMS_2.getMotor(PORT_DUO_SET_2);
        MOTOR_DUO_CAL_2 = AFMS_2.getMotor(PORT_DUO_CAL_2);
        if (scan_for_driver(DRIVER_2) == true)
        {
            driver_present.AFMS_2_Present = true;
            AFMS_2.begin();
        }
        else
        {
            driver_present.AFMS_2_Present = false;
        }
    }

    reset_position();

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
    if (PWM_duty_cycle_percent == _current_speed_percent)
    {
    }
    else
    {
        uint8_t PWM;
        // PWM = (PWM_duty_cycle_percent >= 20) ? 51 : static_cast<uint8_t>((static_cast<float>(PWM_duty_cycle_percent) / 100.0) * 255); // limiter le PWM a 20% goes from percent to uint8

        PWM = (PWM_duty_cycle_percent >= 100) ? 255 : static_cast<uint8_t>((static_cast<float>(PWM_duty_cycle_percent) / 100.0) * 255); // goes from percent to uint8
        _current_speed_percent = PWM;
        if (driver_present.AFMS_0_Present)
        {
            MOTOR_PRIM_DUO_ON_OFF->setSpeed(PWM);
            MOTOR_PRIM_DUO_SET->setSpeed(PWM);
            MOTOR_PRIM_DUO_CAL->setSpeed(PWM);
            MOTOR_PROPILOT_MENU_ESC->setSpeed(PWM);
        }
        if (driver_present.AFMS_1_Present)
        {
            MOTOR_PROPILOT_SET->setSpeed(PWM);
            MOTOR_PROPILOT_UP->setSpeed(PWM);
            MOTOR_PROPILOT_DOWN->setSpeed(PWM);
            MOTOR_PROPILOT_CAL_OK->setSpeed(PWM);
        }
        if (driver_present.AFMS_2_Present)
        {
            MOTOR_DUO_ON_OFF_2->setSpeed(PWM);
            MOTOR_DUO_SET_2->setSpeed(PWM);
            MOTOR_DUO_CAL_2->setSpeed(PWM);
        }
    }
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

void DRIVER::reset_position()
{

    const uint8_t time_delay = 200;
    set_speed(100 /* % percent*/);
    if (driver_present.AFMS_0_Present)
    {
        for (uint8_t i = 0; i <= 3; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(time_delay);
        }
    }
    if (driver_present.AFMS_1_Present)
    {
        for (uint8_t i = 4; i <= 7; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(time_delay);
        }
    }
    if (driver_present.AFMS_2_Present)
    {
        for (uint8_t i = 8; i <= 11; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(time_delay);
        }
    }
    Serial.println("ACUTATOR POSITION RESET");
}

void DRIVER::move_close(uint8_t button)
{
    // TODO move all acutators closer here.
    // TODO implement the waiting for test to start, and when the test starts go to next step;
    if (/*test started*/ 1)
    {
    }

    task_BUTTON[button].current_state = MOVE_FWD_CLOSE;
}

void DRIVER::press_button(uint8_t button)
{
    task_BUTTON[button].current_state = PRESS_BUTTON_SLOW;
}

void DRIVER::ACUTATOR_cycle(uint8_t button)
{
    switch (task_BUTTON[button].current_state)
    {
        //* ***************************************************************** case POS_0_IDLE ************************************************
    case POS_0_IDLE:

        break;
        //* ***************************************************************** case MOVE_FWD_CLOSE ************************************************
    case MOVE_FWD_CLOSE:
        set_speed(100);
        static unsigned long startTime_move_fwd_fast = millis();
        move_fwd(button); // Ensure this function is non-blocking

        // Check elapsed time
        if (millis() - startTime_move_fwd_fast >= 450 /* ms*/)
        {
            Serial.println();
            release(button);
            task_BUTTON[button].current_state = MOVE_CLOSE_DONE;
            // Reset startTime if needed, or perform any necessary cleanup
            startTime_move_fwd_fast = 0;
        }
        break;

        //* ***************************************************************** case MOVE_CLOSE_DONE ************************************************
    case MOVE_CLOSE_DONE:
        set_speed(50);
        task_BUTTON[button].current_state = READY_TO_PRESS;
        break;

        //* ***************************************************************** case READY_TO_PRESS ************************************************
    case READY_TO_PRESS:
        break;
        //* ***************************************************************** case PRESS_BUTTON_SLOW ************************************************
    case PRESS_BUTTON_SLOW:
        if (task_BUTTON[button].startTime_press == 0)
        {
            task_BUTTON[button].startTime_press = millis();
        }
        move_fwd(button);
        if (millis() - task_BUTTON[button].startTime_press >= 400)
        {
            release(button); // maybe doesent need to release to keep the force.
            task_BUTTON[button].current_state = PRESSED;
            task_BUTTON[button].startTime_press = 0;
        }
        break;

        //* ***************************************************************** case PRESS_BUTTON_SLOW ************************************************

        //* ***************************************************************** case PRESSED ************************************************
    case PRESSED:

        if (task_BUTTON[button].releaseTime == 0)
        {
            task_BUTTON[button].releaseTime = millis();
        }
        if (millis() - task_BUTTON[button].releaseTime >= 200)
        { // Press button 500ms
            task_BUTTON[button].releaseTime = 0;
            task_BUTTON[button].current_state = BUTTON_UNPRESS;
        }
        break;

        //* ***************************************************************** case PRESS_BUTTON_UNPRESS ************************************************
    case BUTTON_UNPRESS:
        if (task_BUTTON[button].unpressTime == 0)
        {
            task_BUTTON[button].unpressTime = millis();
        }
        move_bwd(button);

        release(button);
        if (millis() - task_BUTTON[button].unpressTime >= 30)
        {
            task_BUTTON[button].unpressTime = 0;
            release(button); // maybe doesent need to release to keep the force.
            task_BUTTON[button].current_state = UNPRESSED;
        }
        break;

        //* ***************************************************************** case BUTTON_UNPRESS ************************************************

        //* ***************************************************************** case UNPRESSED ************************************************
    case UNPRESSED:
        task_BUTTON[button].current_state = READY_TO_PRESS; //? return to ready to be pressed?
        break;

        //* ***************************************************************** case RETURN_POS_0 ************************************************
    case RETURN_POS_0:
        set_speed(100);
        task_BUTTON[button].current_state = POS_0_IDLE;

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
