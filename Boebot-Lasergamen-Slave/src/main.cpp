#include "main.h"

struct PS4 PS4InputsMain;

void Task1code(void* parameter);
void Task2code(void* parameter);

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  pinMode(12, OUTPUT);
  

    PS4.setLed(255, 0, 0);

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
  // put your main code here, to run repeatedly:
   
  }

void Task1code( void * parameter) // Taken voor core 0
{
  // Setup
  
  while(1) 
  {
    /*
    
    Core 0:

    Voor de camera 

    @daniel 
    
    
    */

  }
}

void Task2code( void * parameter) // Taken voor core 1
{
  // Setup
  initMotors(0);
  Serial.println("Hello World");
  PS4.begin("80:ea:23:1b:fc:e7");

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
    }
    else
    {
      digitalWrite(12, LOW);
    }
    

    
    delay(10);
  }
}

