#include "main.h"
#include "esp32-hal-spi.h"


void Task1code(void* parameter);
void Task2code(void* parameter);


TaskHandle_t Task1;
TaskHandle_t Task2;

volatile byte Shoot = 0;
volatile byte Flag_error_handler = 0;
volatile byte error = 0;

byte Teamkleur[3];
//uninitalised pointers to SPI objects

Stepper myStepper = Stepper(STEPSPERREVOLUTION, 18, 23, 19, 5);

void setup() {
  // put your setup code here, to run once:
  int i = 0;

  
 
  Serial.begin(115200); 

  while (1)
  {
  myStepper.setSpeed(15);
	myStepper.step(STEPSPERREVOLUTION);
	delay(2000);
  myStepper.step(-STEPSPERREVOLUTION);
	delay(1000);
  
  }
  PS4.begin("70:EA:23:1B:FC:FF");

  Serial.println("Waiting for controller:");
  // while(!PS4.isConnected())    
  // { 
  //   if (i == 300)
  //   {

  //     UI_layer_error_handling(CONTROLLERNOTDETECTED);
  //     i = 0;
  //   }
  //   i++;

  //   delay(100);
  // }
  Serial.println("Controller detected:\n");

  initSPI();  // Initialise SPI
  i = 1;

  Serial.println("Starting SPI");
  int state = 0;
  byte data[3] = {0,0,0}; 
  byte* result = 0;
  
  delay(100);


  // start a state diagram for start of the SPI communication
  // This can be blocking to allow Willem to start correct
  // 
  while (state != 5)
  {
    switch (state)
    {
      case 0: // Ask the ESPCAM for a start
      {
        result = Logiclayer_SPI_CMD(STARTGAME, data); // Verwacht hier geen echte data uit

        delay(TIMEBETWEENCMDS);
        state = 1;
        break;

      }
      case 1: // Vraag om de status voor evt errors
      {
        result = Logiclayer_SPI_CMD(STATUSSLAVE, data); // Hier krijg je een ack

        if (result[0] == SLAVEACKNOWLEDGE) 
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
        Serial.println("Line: 2\n");

        result = Logiclayer_SPI_CMD(TEAMCOLOUR, data); // verwacht hier de status van 
        // Doe hier wat met de data uit de cam!

        if (result[0] == ERROR)
        {
           UI_layer_error_handling (result[1]);
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

        result = Logiclayer_SPI_CMD(PLACEMAKER, data);
        Serial.print(data[0], DEC);
        Teamkleur[0] = result[1];
        Teamkleur[1] = result[2];
        Teamkleur[2] = result[3];
        
        state = 5; // Eindig de while loop

      }


    }

  }

  // Functie om de kleur van de esp32 te veranderen naar de corresponderende kleur

  


  // Init functie voor communicatie met de esp-cam naar de 



  xTaskCreatePinnedToCore
  (
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    20000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0 /* Core where the task should run */
  ); 

  xTaskCreatePinnedToCore
  (
    Task2code, /* Function to implement the task */
    "Task2", /* Name of the task */
    20000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    1,  /* Priority of the task */
    &Task2,  /* Task handle. */
    1  /* Core where the task should run */
  );
}

void loop() { 
}

void Task1code( void * parameter) // Taken voor core 0
{
  // Setup
  byte* Flag_SPI = nullptr; 
  

  
  Flag_SPI = InitTimerInterrupt(PRESCALER, TIMERTICKS); // Init Timer interrupt returns a flag pointer 

  byte junk = 0;

  while(1) 
  {
    /*
      
    
    */

   if (Flag_SPI != nullptr && *Flag_SPI == 1 && Shoot != 0)
  {
    if (junk == 255) 
    {
      junk = 1;
    }
    else
    {
      junk++;
    }

    Shoot = 0;
    *Flag_SPI = 0;
  }

       delay(10);
  }
}

void Task2code( void * parameter) // Taken voor core 1
{
  // Setup
  struct PS4 PS4InputsMain;
  initMotors(0);
 
  int i = 5;
  

  
  while(1)
  {
    PS4InputsMain = IO_Layer_Besturing();
    PS4InputsMain = Logiclayer_Besturing_Data(PS4InputsMain);
    
    updateMotor(motorLinks, PS4InputsMain.MotordataLinks);
    updateMotor(motorRechts, PS4InputsMain.MotordataRechts);
    
    // if (PS4.R2Value() > 20)
    // {
    //   PS4.setRumble(PS4.R2Value(), PS4.R2Value());
    //   PS4.sendToController();
    // }

    if (PS4InputsMain.Cirkelknop == true) // Zet een timer neer
    {
      digitalWrite(12, HIGH);
      Shoot = 0xAA;
    }
    else
    {
      digitalWrite(12, LOW);
    }
    
    delay(10);
  }
}


