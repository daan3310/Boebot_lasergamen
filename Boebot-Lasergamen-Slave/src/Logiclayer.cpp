#include "Logiclayer.h"

// char* shutdown = "shutdown.";
// char* startgame = "startgame.";
// char* placemaker = "placemaker.";
// char* statusslave = "statusslave.";
// char* teamcolour = "teamcolour.";
// char* errorc = "error.";
// char* shoot = "shoot.";
// char* errorc1 = "error";
// char* acknowledge = "acknowledge";
byte shutdown = 1;
byte placemaker = 2;
byte startgame = 3;
byte statusslave = 4;
byte shoot =  5;
byte errorc = 6;
byte teamcolour = 7;
byte acknowledge = 8;
byte error1 = 9;
char** Serial_Data_In;
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

byte Logiclayer_Serial_CMD(byte CMD)
{
  switch (CMD)
  {
    case SHUTDOWN: //1
    {
      
      return serial_send_command(shutdown);
    }
    case PLACEMAKER: //2
    {
      
      return serial_send_command(placemaker);
    }
    case STARTGAME: //3
    {
      
      return serial_send_command(startgame);
    }
    case ACKNOWLEDGE:
    {
      break;
    }
    case STATUSSLAVE:
    {
      
      return serial_send_command(statusslave);
    }
    case SHOOT:
    {
      
      return serial_send_command(shoot);
    }
    case ERROR:
    {
      
      return serial_send_command(errorc);
    }
    case TEAMCOLOUR:
    {
      
      return serial_send_command(teamcolour);
    }
    default:
    {
      UI_layer_error_handling(NOCMD);
    }
  }
  return 0;
}

byte Logiclayer_Serial_CMD_NO_DATA(byte CMD)
{
  byte* datain;
  byte data[DATALENGTH-1] = {0,0,0};

  switch (CMD)
  {
    case SHUTDOWN:
    {
      return serial_send_command(shutdown);
    }
    case PLACEMAKER:
    {
      return serial_send_command(placemaker);
    }
    case STARTGAME:
    {
      return serial_send_command(startgame);
    }
    case ACKNOWLEDGE:
    {
      break;
    }
    case STATUSSLAVE:
    {
      return serial_send_command(statusslave);
    }
    case SHOOT:
    {
      //Serial.println("Shoot CASE.");
      return serial_send_command(shoot);
    }
    case ERROR:
    {
      return serial_send_command(errorc);
    }
    case TEAMCOLOUR:
    {
      return serial_send_command(teamcolour);
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

byte Logiclayer_Startup_Serial(byte state)
{
  byte data[DATALENGTH-1] = {0x00,0x00,0x00}; 

switch (state)
    {
      case 0: // Ask the ESPCAM for a start
      {
       
        Logiclayer_Serial_CMD(STARTGAME); // Verwacht hier geen echte data uit
        Function_Print_Serial_input(state);
        //Function_Print_Spi_input(state);
        delay(TIMEBETWEENCMDS);
        state = 1;
        break;

      }
      case 1: // Vraag om de status voor evt errors
      {
        Logiclayer_Serial_CMD(STATUSSLAVE); // Hier krijg je een ack
        Function_Print_Serial_input(state);
        //Function_Print_Spi_input(state);

        if (My_Serial_dataIn == acknowledge) 
        {
          delay(TIMEBETWEENCMDS);
          state = 2;
        }
        else // als het geen ack is, is er iets fout gegaan bij de CAM
        {
          UI_layer_error_handling (ESPSLAVENOTDETECTED); 
          state = 0;  // Reset de state machine
          delay(TIMEBETWEENCMDS);
        }
        delay(TIMEBETWEENCMDS);
        break;
      }
      case 2: // Hier stuur je de vraag welk team wij zijn
      {
        Logiclayer_Serial_CMD(TEAMCOLOUR); // verwacht hier de status van 
        // Doe hier wat met de data uit de cam!

        Function_Print_Serial_input(state);

        if (My_Serial_dataIn == error1)
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
        Logiclayer_Serial_CMD(PLACEMAKER);
        
        //Function_Print_Spi_input(state);


        
        state = 5; // Eindig de while loop

      }


    }
  return state;
  }