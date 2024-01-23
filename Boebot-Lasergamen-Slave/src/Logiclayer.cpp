#include "Logiclayer.h"


struct PS4 Logiclayer_Besturing_Data(struct PS4 PS4Inputs)
{
  uint ServoHoek = 0;
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

  if (PS4Inputs.Rechterjoystick_x > STICKDRIFT || PS4Inputs.Rechterjoystick_x < -STICKDRIFT || PS4Inputs.Rechterjoystick_y > STICKDRIFT || PS4Inputs.Rechterjoystick_y < -STICKDRIFT)
  {

      // ADD features for Turret
  }
  else 
  {

    PS4Inputs.Rechterjoystick_x = 0;
    PS4Inputs.Rechterjoystick_y = 0;
  
  }
  return PS4Inputs; 
}

byte* Logiclayer_SPI_CMD(int CMD, byte data[3])
{
  byte* datain;
  if (CMD >= AMOUNTOFCMD)
  {

  } 
  switch (CMD)
  {
    case SHUTDOWN:
    {
      return hspi_send_command(SHUTDOWN, data);
    }
    case PLACEMAKER:
    {
      return hspi_send_command(PLACEMAKER, data);
    }
    case STARTGAME:
    {
      return hspi_send_command(STARTGAME, data);
    }
    case ACKNOWLEDGE:
    {
      break;
    }
    case STATUSSLAVE:
    {
      return hspi_send_command(STARTGAME, data);
    }
    case SHOOT:
    {
      return hspi_send_command(SHOOT, data);
    }
    case ERROR:
    {
      return hspi_send_command(ERROR, data);
    }
    
  }
  return 0;
}

void Logiclayer_set_colour(byte Color[3])
{
// functie om ledstrip 


PS4.setLed(Color[0], Color[1], Color[2]);

PS4.sendToController();


}
