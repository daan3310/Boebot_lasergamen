#ifndef MAIN_H
#define MAIN_H

// Arduino Libraries
#include <Arduino.h>
#include "PS4Controller.h"
#include <math.h>
#include <SPI.h>

#include <Stepper.h>


// User Libraries
#include "IO-layer.h"
#include "Logiclayer.h"
#include "UI-layer.h"
#include <Wireless_Communication/Wireless_Communication.h>

#define PRESCALER 16000  //16000
#define TIMERTICKS 5000

#define TIMEBETWEENCMDS 1000

#define STEPSPERREVOLUTION 2048


void Task1code( void * parameter);
void Task2code( void * parameter);


#endif