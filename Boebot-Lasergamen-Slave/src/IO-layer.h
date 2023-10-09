#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"
#include <ESP32Servo.h>

// any motor added to the system should be added to this enum "motor" and the switch case in the "updateMotor" function in IO-layer.cpp
enum motor { motorRechts, motorLinks, motorTurret };
#define motorRechtsPWM  5
#define motorRechtsDIR  19
#define motorLinksPWM   23
#define motorLinksDIR   18
#define motorTurretPWM  13
// this function accepts values from -1024 to 1024 and sets the motor to this value
// values outside this range are clamped to stay within valid values
uint initMotors(int timer);
uint updateMotor(motor currentMotor, int motorPower);

#endif 


/*
#ifdef ESP32
#define A0 2
#define A1 4
#define A2 35
#define A3 34
#define A4 36
#define A5 39
#define A6 32
#define A7 33
#define D2 26
#define D3 25
#define D4 7
#define D5 16
#define D6 17
#define D7 14
#define D8 12
#define D9 13
#define D10 5
#define D11 23
#define D12 19
#define D13 18
#endif
*/