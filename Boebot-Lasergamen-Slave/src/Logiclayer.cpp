#include "Logiclayer.h"


int Logiclayer_Besturing_Data(struct PS4 PS4Inputs)
{
  PS4Inputs.MotordataLinks = PS4Inputs.R2 * 4;
  PS4Inputs.MotordataRechts = PS4Inputs.R2 * 4;
  

  PS4Inputs.MotordataLinks += -PS4Inputs.L2 * 4;
  PS4Inputs.MotordataRechts += -PS4Inputs.L2 * 4;
  

  if (PS4Inputs.Linkerjoystick_x > STICKDRIFT || PS4Inputs.Linkerjoystick_x < -STICKDRIFT)
  {

    PS4Inputs.MotordataLinks -= PS4Inputs.Linkerjoystick_x * 4;
    PS4Inputs.MotordataRechts += PS4Inputs.Linkerjoystick_x * 4;
    
  }
  else 
  {
    PS4Inputs.Linkerjoystick_x = 0;
    PS4Inputs.Linkerjoystick_y = 0;
  }

  if (PS4Inputs.Rechterjoystick_x > STICKDRIFT || PS4Inputs.Rechterjoystick_x < -STICKDRIFT)
  {

    Logiclayer_Servohoek_berekenen(PS4Inputs.Rechterjoystick_x, PS4Inputs.Rechterjoystick_y);

  }
  else 
  {
    PS4Inputs.Rechterjoystick_x = 0;
    PS4Inputs.Rechterjoystick_y = 0;
  }
  return 0; 
}

int8_t Logiclayer_Servohoek_berekenen(int8_t x, int8_t y)
{
  int8_t ServoHoek = 0;
  double ServoHulp = 0;
  ServoHulp = atan2(y,x);
  
  ServoHoek = ServoHulp * 180.0 / M_PI;
  Serial.printf("De hoek is: %d",ServoHoek);
  Serial.println();
  return ServoHoek;
}


