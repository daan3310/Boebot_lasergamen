#include "IO-layer.h"

#define SERIAL_BAUD_RATE 115200  // Change baud rate as needed

Servo myMotorTurret;
SPIClass * vspi = NULL;

byte My_Flag_SPI;

byte My_Serial_dataIn[4] = {0,0,0,0};



void IRAM_ATTR Timer0_ISR()
{
  My_Flag_SPI = 1;
}


uint initMotors(int timer)
{
    uint returnValue = 0;
    
    ESP32PWM::allocateTimer(timer);
    myMotorTurret.setPeriodHertz(50);
    myMotorTurret.attach(motorTurretPWM, 500, 2500);
    
    digitalWrite(motorRechtsPWM, LOW);  // set speed to 0
    digitalWrite(motorRechtsDIR, HIGH); // set direction to forward
    digitalWrite(motorLinksPWM, LOW);   // set speed to 0
    digitalWrite(motorLinksDIR, HIGH);  // set direction to forward

    pinMode(motorRechtsPWM, OUTPUT);
    pinMode(motorRechtsDIR, OUTPUT);
    pinMode(motorLinksPWM, OUTPUT);
    pinMode(motorLinksDIR, OUTPUT);

    return returnValue;
}

uint updateMotor(motor currentMotor, int motorPower)
{
    uint returnValue = 0;
    bool direction = (0 <= motorPower);

    // check if motorPower exceeds it's bounds of 1000 to -1000, if so change returnValue accordingly
    if(-1024 > motorPower || 1024 < motorPower)
    {
        if(0 == direction)
        {
            // motorPower = 1024;
            // delay(1);
            motorPower = -1024;
            //returnValue = updateMotor_exceeded_lower_bound_of_motorPower;
        }
        else
        {
            // motorPower = -1024;
            // delay(1);
            motorPower = 1024;
            //returnValue = updateMotor_exceeded_upper_bound_of_motorPower;
        }
    }

    // any motor added to this switch case should also be added to the enum "motor in IO-layer.h"
    switch(currentMotor)
    {
        case motorRechts:
            analogWrite(motorRechtsPWM, abs(motorPower) / 4);
            digitalWrite(motorRechtsDIR, direction); // might need to add ! to invert the direction
            break;
        case motorLinks:
            analogWrite(motorLinksPWM, abs(motorPower) / 4);
            digitalWrite(motorLinksDIR, direction); // might need to add ! to invert the direction
            break;
        case motorTurret:
            myMotorTurret.write(1500 + (motorPower));
            break;
        default:
            //returnValue = updateMotor_invalid_value_for_currentMotor;
            break;
    }
    return returnValue;
}

struct PS4 IO_Layer_Besturing()
{
    struct PS4 PS4Inputs;
  if (PS4.isConnected()) 
  {
    PS4Inputs.Cirkelknop = PS4.Circle();
    PS4Inputs.R2 = PS4.R2Value();
    PS4Inputs.L2 = PS4.L2Value();
    PS4Inputs.Linkerjoystick_x = PS4.LStickX();
    PS4Inputs.Linkerjoystick_y = PS4.LStickY();
    PS4Inputs.Rechterjoystick_x = PS4.RStickX();
    PS4Inputs.Rechterjoystick_y = PS4.RStickY();
    return PS4Inputs;
   }       

    return PS4Inputs; 
}

// byte initSerial() {
//   //Serial.begin(SERIAL_BAUD_RATE);  // Initialize serial communication
//   Serial.begin(9600);
// }

byte serial_send_command(byte cmd, byte data[DATALENGTH-1]) {
  #if DEBUG > 0
  Function_Print_Serial_output(cmd, data);
  #endif

  byte dataOut[DATALENGTH];
  byte dataIn[DATALENGTH];

  dataOut[0] = cmd;
  for (int i = 0; i < DATALENGTH - 1; i++) {
    dataOut[i + 1] = data[i];
  }

  // Send data out through serial
  Serial.write(dataOut, DATALENGTH);

  // Read incoming data if needed
  if (Serial.available() >= DATALENGTH) {
    //Serial.println("Datatest:");
    //Serial.readBytes(dataIn, DATALENGTH);
    for (int i = 0; i < DATALENGTH; i++) {
      dataIn[i] = Serial.read(); 
      Serial.print(dataIn[i]);
      Serial.print(", ");
    }
    
  }
  // Copy received data to your buffer
  for (int i = 0; i < DATALENGTH; i++) {
    My_Serial_dataIn[i] = dataIn[i];
  }

  return 0;
}




void InitTimerInterrupt(uint Prescaler, uint TimerTicks)
{
    hw_timer_t *Timer0_Cfg = NULL;

    Timer0_Cfg = timerBegin(0, Prescaler, true); 
    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
    timerAlarmWrite(Timer0_Cfg, TimerTicks, true); 
    timerAlarmEnable(Timer0_Cfg);
}

void InitLedstrip()
{


}

void InitStepperMotor()
{
    
}

