#include "IO-layer.h"

Servo myMotorTurret;
SPIClass * vspi = NULL;

byte Flag_SPI;

void IRAM_ATTR Timer0_ISR()
{
  Flag_SPI = 1;
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
            motorPower = -1024;
            //returnValue = updateMotor_exceeded_lower_bound_of_motorPower;
        }
        else
        {
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
byte initSPI()
{
  vspi = new SPIClass(VSPI);  
  
  vspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

  pinMode(HSPI_SS, OUTPUT); //HSPI SS

  return 0;
}

byte* hspi_send_command(byte cmd, byte data[3]) 
{  
  byte dataOut[4]; // junk data to illustrate usage
  
  static byte dataIn[4];
  
  dataOut[0] = cmd;
  for (int i = 0; i<3;i++)
  {
    dataOut[i+1] = data[i];
  }

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));

  digitalWrite(HSPI_SS, LOW); //pull SS low to prep other end for transfer
  delay(10);

  
  vspi->transferBytes(dataIn, dataOut, 4);  

  delay(10);
  digitalWrite(HSPI_SS, HIGH); //pull ss high to signify end of data transfer

  vspi->endTransaction();

  return dataIn;

}



byte* InitTimerInterrupt(uint Prescaler, uint TimerTicks)
{
    byte* pFlag_SPI = &Flag_SPI;

    hw_timer_t *Timer0_Cfg = NULL;

    Timer0_Cfg = timerBegin(0, Prescaler, true); 
    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
    timerAlarmWrite(Timer0_Cfg, TimerTicks, true); 
    timerAlarmEnable(Timer0_Cfg);
    
    return pFlag_SPI;
}

void InitLedstrip()
{


}

