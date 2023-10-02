#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"

// any motor added to the system should be added to this enum "motor" and the switch case in the "updateMotor" function in IO-layer.cpp
enum motor { motorRechts, motorLinks};//, motorTurret };
// this function accepts values from -1000 to 1000 and sets the motor to this value
// values outside this range are clamped to stay within valid values
uint updateMotor(motor currentMotor, int motorPower);

#endif 
