#include "IO-layer.h"

uint updateMotor(motor currentMotor, int motorPower)
{
    uint returnValue = 0;
    bool direction = (0 <= motorPower);

    // check if motorPower exceeds it's bounds of 1000 to -1000, if so change returnValue accordingly
    if(-1000 > motorPower || 1000 < motorPower)
    {
        if(0 == direction)
        {
            motorPower = -1000;
            //returnValue = updateMotor_exceeded_lower_bound_of_motorPower;
        }
        else
        {
            motorPower = 1000;
            //returnValue = updateMotor_exceeded_upper_bound_of_motorPower;
        }
    }

    // any motor added to this switch case should also be added to the enum "motor in IO-layer.h"
    switch(currentMotor)
    {
        case motorRechts:
            break;
        case motorLinks:
            break;
        default:
            //returnValue = updateMotor_invalid_value_for_currentMotor;
            break;
    }
    return returnValue;
}
