#include "Logiclayer.h"

struct PS3 Logiclayer_Besturing_Data(struct PS3 PS3inputs)
{
  uint ServoHoek = 0;
  PS3inputs.MotordataLinks = PS3inputs.R2 * 4;
  PS3inputs.MotordataRechts = PS3inputs.R2 * 4;
  

  PS3inputs.MotordataLinks += -PS3inputs.L2 * 4;
  PS3inputs.MotordataRechts += -PS3inputs.L2 * 4;
  

  if (PS3inputs.Linkerjoystick_x > STICKDRIFT || PS3inputs.Linkerjoystick_x < -STICKDRIFT)
  {

    PS3inputs.MotordataLinks -= PS3inputs.Linkerjoystick_x * 4;
    PS3inputs.MotordataRechts += PS3inputs.Linkerjoystick_x * 4;
    
  }
  else 
  {
    PS3inputs.Linkerjoystick_x = 0;
    PS3inputs.Linkerjoystick_y = 0;
  }

  if (PS3inputs.Rechterjoystick_x > STICKDRIFT || PS3inputs.Rechterjoystick_x < -STICKDRIFT || PS3inputs.Rechterjoystick_y > STICKDRIFT || PS3inputs.Rechterjoystick_y < -STICKDRIFT)
  {

      // ADD features for Turret
  }
  else 
  {

    PS3inputs.Rechterjoystick_x = 0;
    PS3inputs.Rechterjoystick_y = 0;
  
  }
  return PS3inputs; 
}
byte Logiclayer_SPI_CMD(byte CMD, byte data[3])
{
  byte* datain;
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
      return hspi_send_command(STATUSSLAVE, data);
    }
    case SHOOT:
    {
      return hspi_send_command(SHOOT, data);
    }
    case ERROR:
    {
      return hspi_send_command(ERROR, data);
    }
    case TEAMCOLOUR:
    {
      return hspi_send_command(TEAMCOLOUR, data);
    }
    default:
    {
      UI_layer_error_handling(NOCMD);
    }
  }
  return 0;
}

byte Logiclayer_SPI_CMD_NO_DATA(byte CMD)
{
  byte* datain;
  byte data[DATALENGTH-1] = {0,0,0};

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
      return hspi_send_command(STATUSSLAVE, data);
    }
    case SHOOT:
    {
      return hspi_send_command(SHOOT, data);
    }
    case ERROR:
    {
      return hspi_send_command(ERROR, data);
    }
    case TEAMCOLOUR:
    {
      return hspi_send_command(TEAMCOLOUR, data);
    }
    
  }
  return 0;
}


byte Logiclayer_Startup_SPI(byte state)
{
  byte data[DATALENGTH-1] = {0x00,0x00,0x00}; 

switch (state)
    {
      case 0: // Ask the ESPCAM for a start
      {
       
        Logiclayer_SPI_CMD(STARTGAME, data); // Verwacht hier geen echte data uit
        
        //Function_Print_Spi_input(state);
        delay(TIMEBETWEENCMDS);
        state = 1;
        break;

      }
      case 1: // Vraag om de status voor evt errors
      {
        Logiclayer_SPI_CMD(STATUSSLAVE, data); // Hier krijg je een ack

        //Function_Print_Spi_input(state);

        if (My_SPI_dataIn[0] == ACKNOWLEDGE) 
        {
          delay(TIMEBETWEENCMDS);
          state = 2;
        }
        else // als het geen ack is, is er iets fout gegaan bij de CAM
        {
          UI_layer_error_handling (ESPSLAVENOTDETECTED); // Geen CAM gedetecteerd op SPI
          state = 0;  // Reset de state machine
          delay(TIMEBETWEENCMDS);
        }
        break;
      }
      case 2: // Hier stuur je de vraag welk team wij zijn
      {
        Logiclayer_SPI_CMD(TEAMCOLOUR, data); // verwacht hier de status van 
        // Doe hier wat met de data uit de cam!

        Function_Print_Spi_input(state);

        if (My_SPI_dataIn[0] == ERROR)
        {
           //UI_layer_error_handling (result[1]);
        }
        else
        {
          state = 3;
        }
        
        delay(TIMEBETWEENCMDS);
        break;
      }
      case 3: // Placemaker om te vragen welke kleur/team wij zijn
      {
        Logiclayer_SPI_CMD(PLACEMAKER, data);
        
        //Function_Print_Spi_input(state);


        
        state = 5; // Eindig de while loop

      }


    }
  return state;
  }
