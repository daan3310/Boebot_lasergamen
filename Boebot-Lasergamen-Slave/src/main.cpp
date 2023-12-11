#include "main.h"

struct PS4 PS4InputsMain;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  //PS4.begin("5c:96:56:b2:fb:c6");
  PS4.begin("80:ea:23:1b:fc:e7");
  initMotors(0);

  xTaskCreatePinnedToCore
  (
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
      0 /* Core where the task should run */
      ); 

  xTaskCreatePinnedToCore
  (
    Task2code, /* Function to implement the task */
    "Task2", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task2,  /* Task handle. */
    1  /* Core where the task should run */
    );
}

void loop() {
  // put your main code here, to run repeatedly:
    PS4InputsMain = IO_Layer_Besturing();
    PS4InputsMain = Logiclayer_Besturing_Data(PS4InputsMain);
    
    updateMotor(motorLinks, PS4InputsMain.MotordataLinks);
    updateMotor(motorRechts, PS4InputsMain.MotordataRechts);
    
    if (PS4InputsMain.Cirkelknop == true) // Zet een timer neer
    {
      Serial.print("Schiet");
    }
    
    delay(10);
  }

void Task1code( void * parameter) // Taken voor core 0
{
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
  while(1)
  {

  }
}

