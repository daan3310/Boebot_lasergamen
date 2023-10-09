#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"
#include <ESP32Servo.h>

// any motor added to the system should be added to this enum "motor" and the switch case in the "updateMotor" function in IO-layer.cpp
enum motor { motorRechts, motorLinks, motorTurret };
#define motorRechtsPWM 5
#define motorRechtsDIR 19
#define motorLinksPWM 23
#define motorLinksDIR 18
#define motorTurretPWM 13
// this function accepts values from -1024 to 1024 and sets the motor to this value
// values outside this range are clamped to stay within valid values
uint initMotors(int timer);
uint updateMotor(motor currentMotor, int motorPower);

#endif 
