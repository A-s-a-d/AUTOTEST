#include "PQ12.h"

PQ12::PQ12(/* args */)
{
}

void PQ12::begin()
{
    AFMS.begin();
    myMotor->setSpeed(100);
}
void PQ12::move_fwd()
{
    myMotor->run(FORWARD);
}
void PQ12::move_bwd()
{
    myMotor->run(BACKWARD);
}
void PQ12::dtct_press()
{
}
void PQ12::dtct_proximity()
{
}
void PQ12::move_closer()
{
}
void PQ12::press_ONOFF()
{
}
void PQ12::press_SET()
{
}
void PQ12::press_CAL()
{
}