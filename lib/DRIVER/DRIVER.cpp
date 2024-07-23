
#include "DRIVER.h"

DRIVER::DRIVER(/* args */)
{
}

void DRIVER::begin()
{
    AFMS.begin();
    myMotor->setSpeed(100);
}
void DRIVER::move_fwd()
{
    
    myMotor->run(FORWARD);
}
void DRIVER::move_bwd()
{
    myMotor->run(BACKWARD);
}
void DRIVER::dtct_press()
{
}
void DRIVER::dtct_proximity()
{
}
void DRIVER::move_closer()
{
}
void DRIVER::press_ONOFF()
{
}
void DRIVER::press_SET()
{
}
void DRIVER::press_CAL()
{
}