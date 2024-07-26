
#include "DRIVER.h"

DRIVER::DRIVER() :
#if MOTOR_TYPE == DC
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2)
#elif MOTOR_TYPE == STEPPER
                   AFMS_0(DRIVER_0), AFMS_1(DRIVER_1), AFMS_2(DRIVER_2), AFMS_3(DRIVER_3), AFMS_4(DRIVER_4), AFMS_5(DRIVER_5)
#endif
{
    for (uint8_t i = 0; i <= 11; i++)
    {
        task_BUTTON[11].button = i;
    }
    _current_speed_percent = 0;
}

void DRIVER::begin()
{
#if MOTOR_TYPE == DC
    Serial.println(myDEVICE.Device_ID);
    if (myDEVICE.Device_ID == PRIM)
    {
        MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getMotor(PORT_PRIM_DUO_ON_OFF);
        MOTOR_PRIM_DUO_SET = AFMS_0.getMotor(PORT_PRIM_DUO_SET);
        MOTOR_PRIM_DUO_CAL = AFMS_0.getMotor(PORT_PRIM_DUO_CAL);
        AFMS_0.begin();
    }
    else if (myDEVICE.Device_ID == PROPILOT)
    {
        MOTOR_PROPILOT_MENU_ESC = AFMS_0.getMotor(PORT_PROPILOT_MENU_ESC);

        MOTOR_PROPILOT_SET = AFMS_1.getMotor(PORT_PROPILOT_SET);
        MOTOR_PROPILOT_UP = AFMS_1.getMotor(PORT_PROPILOT_UP);
        MOTOR_PROPILOT_DOWN = AFMS_1.getMotor(PORT_PROPILOT_DOWN);
        MOTOR_PROPILOT_CAL_OK = AFMS_1.getMotor(PORT_PROPILOT_CAL_OK);
        AFMS_0.begin();
        AFMS_1.begin();
    }
    else if (myDEVICE.Device_ID == DUO)
    {
        MOTOR_PRIM_DUO_ON_OFF = AFMS_0.getMotor(PORT_PRIM_DUO_ON_OFF);
        MOTOR_PRIM_DUO_SET = AFMS_0.getMotor(PORT_PRIM_DUO_SET);
        MOTOR_PRIM_DUO_CAL = AFMS_0.getMotor(PORT_PRIM_DUO_CAL);
        MOTOR_DUO_ON_OFF_2 = AFMS_2.getMotor(PORT_DUO_ON_OFF_2);
        MOTOR_DUO_SET_2 = AFMS_2.getMotor(PORT_DUO_SET_2);
        MOTOR_DUO_CAL_2 = AFMS_2.getMotor(PORT_DUO_CAL_2);
        AFMS_0.begin();
        AFMS_2.begin();
    }

    // reset_position_ALL();

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
        MOTOR_PRIM_DUO_ON_OFF->setSpeed(PWM);

        MOTOR_PRIM_DUO_SET->setSpeed(PWM);
        MOTOR_PRIM_DUO_CAL->setSpeed(PWM);
        // TODO uncomment when other drivers present.
        // MOTOR_PROPILOT_MENU_ESC->setSpeed(PWM);
        // MOTOR_PROPILOT_SET->setSpeed(PWM);
        // MOTOR_PROPILOT_UP->setSpeed(PWM);
        // MOTOR_PROPILOT_DOWN->setSpeed(PWM);
        // MOTOR_PROPILOT_CAL_OK->setSpeed(PWM);
        // MOTOR_DUO_ON_OFF_2->setSpeed(PWM);
        // MOTOR_DUO_SET_2->setSpeed(PWM);
        // MOTOR_DUO_CAL_2->setSpeed(PWM);
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
    set_speed(100 /* % percent*/);
    if (myDEVICE.Device_ID == PRIM)
    {
        for (uint8_t i = INDEX_PRIM_DUO_ON_OFF; i <= INDEX_PRIM_DUO_CAL; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(500);
        }
    }
    else if (myDEVICE.Device_ID == PROPILOT)
    {
        for (uint8_t i = INDEX_PROPILOT_MENU_ESC; i <= INDEX_PROPILOT_CAL_OK; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(500);
        }
    }
    else if (myDEVICE.Device_ID == DUO)
    {
        for (uint8_t i = INDEX_DUO_ON_OFF_2; i <= INDEX_DUO_CAL_2; i++)
        {
            release(i);
            delay(5);
            move_bwd(i);
            delay(500);
        }
    }
}

void DRIVER::move_close(uint8_t button)
{

    switch (task_BUTTON[button].current_state)
    {
    case POS_0_IDLE:

        // TODO implement the waiting for test to start, and when the test starts go to next step;
        if (/*test started*/ 1)
        {
        }
        task_BUTTON[button].current_state = MOVE_FWD_CLOSE;
        break;
    case MOVE_FWD_CLOSE:
        set_speed(100);

        task_BUTTON[button].startTime_move_fwd_fast = millis();
        move_fwd(button);
        if (millis() - task_BUTTON[button].startTime_move_fwd_fast >= 120)
        { // 150ms for half distance
            release(button);
            task_BUTTON[button].current_state = MOVE_CLOSE_DONE;
        }
        break;
    case MOVE_CLOSE_DONE:
        release(button);
        set_speed(50);
        task_BUTTON[button].current_state = READY_TO_PRESS;
        break;
    case READY_TO_PRESS:
        task_BUTTON[button].current_state = PRESS_BUTTON_SLOW;
        break;
    case PRESS_BUTTON_SLOW:
        task_BUTTON[button].current_state = WAIT_PRESS;
        break;
    case WAIT_PRESS:
        task_BUTTON[button].current_state = PRESSED;
        break;
    case PRESSED:
        task_BUTTON[button].current_state = PRESS_BUTTON_UNPRESS;
        break;
    case PRESS_BUTTON_UNPRESS:
        task_BUTTON[button].current_state = WAIT_UNPRESS;
        break;
    case WAIT_UNPRESS:
        task_BUTTON[button].current_state = UNPRESSED;
        break;
    case UNPRESSED:
        task_BUTTON[button].current_state = WAIT_UNPRESS;
        break;
    case RETURN_POS_0:
        task_BUTTON[button].current_state = POS_0_IDLE;
        break;
    }
}

void DRIVER::press_button(uint8_t button)
{
    // const uint16_t timemovfwdms = 150;
    // const uint16_t timemovbwdms = 150;
    // const uint16_t timewait_press = 500;

    // move_fwd(BUTTON);
    // delay(timemovfwdms); //* move fwd fast

    // release(BUTTON);
    // delay(1000);

    // set_speed(40);
    // move_fwd(BUTTON);
    // delay(timewait_press); //* move fwd slow press wait

    // release(BUTTON);
    // delay(200); //* move bwd slow unpress wait

    // move_bwd(BUTTON);
    // delay(timewait_press); //* move bwd slow unpress wait
    // release(BUTTON);
    // delay(1000);

    // set_speed(100);
    // move_bwd(BUTTON);
    // delay(timemovbwdms); //* move bwd fast unpress

    // release(BUTTON);

        switch (pressButtonTask.state)
    {
    case PRESS_BUTTON_IDLE:
        pressButtonTask.state = PRESS_BUTTON_MOVING_FWD;
        pressButtonTask.startTime = millis();
        pressButtonTask.button = button;
        move_fwd(button);
        break;

    case PRESS_BUTTON_MOVING_FWD:
        if (millis() - pressButtonTask.startTime >= 150)
        { // 150ms forward
            release(pressButtonTask.button);
            pressButtonTask.state = PRESS_BUTTON_WAIT;
            pressButtonTask.startTime = millis();
        }
        break;

    case PRESS_BUTTON_WAIT:
        if (millis() - pressButtonTask.startTime >= 1000)
        { // Wait 1000ms
            set_speed(40);
            move_fwd(pressButtonTask.button);
            pressButtonTask.state = PRESS_BUTTON_RELEASE;
            pressButtonTask.startTime = millis();
        }
        break;

    case PRESS_BUTTON_RELEASE:
        if (millis() - pressButtonTask.startTime >= 500)
        { // Press button 500ms
            release(pressButtonTask.button);
            move_bwd(pressButtonTask.button);
            pressButtonTask.state = PRESS_BUTTON_MOVING_BWD;
            pressButtonTask.startTime = millis();
        }
        break;

    case PRESS_BUTTON_MOVING_BWD:
        if (millis() - pressButtonTask.startTime >= 500)
        { // 500ms backward
            release(pressButtonTask.button);
            set_speed(100);
            pressButtonTask.state = PRESS_BUTTON_DONE;
            pressButtonTask.startTime = millis();
        }
        break;

    case PRESS_BUTTON_DONE:
        if (millis() - pressButtonTask.startTime >= 150)
        { // Final backward 150ms
            release(pressButtonTask.button);
            pressButtonTask.state = PRESS_BUTTON_IDLE;
        }
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
