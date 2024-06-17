#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"
#include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <string.h>
// any motor added to the system should be added to this enum "motor" and the switch case in the "updateMotor" function in IO-layer.cpp
enum motor { motorRechts, motorLinks, motorTurret };


extern char** My_Serial_dataIn;
extern byte My_Flag_SPI;



// this function accepts values from -1024 to 1024 and sets the motor to this value
// values outside this range are clamped to stay within valid values
uint initMotors(int timer);
uint updateMotor(motor currentMotor, int motorPower);
void InitTimerInterrupt(uint Prescaler, uint TimerTicks);
//byte initSerial();
byte serial_send_command(char* cmd);





struct PS4 
{
    //R2, L2, Rechterjoystick, Linkerjoystick, Rondje schieten,
    uint8_t R2 = 0;
    uint8_t L2 = 0;

    int8_t Rechterjoystick_x = 0;
    int8_t Rechterjoystick_y = 0;
    int8_t Linkerjoystick_x = 0;
    int8_t Linkerjoystick_y = 0;

    bool Cirkelknop = 0;

    int16_t MotordataLinks = 0; 
    int16_t MotordataRechts = 0;

    int8_t ServoHoek = 0; 

};

struct PS4 IO_Layer_Besturing();


#endif 
