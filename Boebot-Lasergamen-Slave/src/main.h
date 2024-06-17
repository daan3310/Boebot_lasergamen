#ifndef MAIN_H
#define MAIN_H

// Arduino Libraries
#include <Arduino.h>
#include "PS4Controller.h"
#include <math.h>
#include <SPI.h>
#include "Config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <Servo.h>

#include <Stepper.h>


// User Libraries
#include "IO-layer.h"
#include "Logiclayer.h"
#include "UI-layer.h"
#include <Wireless_Communication/Wireless_Communication.h>


void Function_Print_Serial_output(char* CMD);

void Function_Print_Serial_input(int state);

void servodirection(signed char Direction);

void Task1code( void * parameter);
void Task2code( void * parameter);


#endif