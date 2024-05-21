#ifndef MAIN_H
#define MAIN_H

// Arduino Libraries
#include <Arduino.h>
//#include "PS4Controller.h"
#include "Ps3Controller.h"
#include <math.h>
#include <SPI.h>
#include "Config.h"

#include <Stepper.h>


// User Libraries
#include "IO-layer.h"
#include "Logiclayer.h"
#include "UI-layer.h"
#include <Wireless_Communication/Wireless_Communication.h>


void Function_Print_Spi_output(byte CMD, byte data[3] );

void Function_Print_Spi_input(int );

void Set_Stepper_direction(signed char Direction);

void Task1code( void * parameter);
void Task2code( void * parameter);


#endif