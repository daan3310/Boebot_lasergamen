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

  if (PS4Inputs.Linkerjoystick_x > STICKDRIFT || PS4Inputs.Linkerjoystick_x < -STICKDRIFT)
  {


  }
  else 
  {
    PS4Inputs.Linkerjoystick_x = 0;
    PS4Inputs.Linkerjoystick_y = 0;
  }
  return 0; 
}


