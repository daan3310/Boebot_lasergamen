#include "main.h"
#include "esp32-hal-spi.h"

void Task1code(void* parameter);
void Task2code(void* parameter);

TaskHandle_t Task1;
TaskHandle_t Task2;

volatile byte Shoot = 0;
volatile byte Flag_error_handler = 0;
volatile byte error = 0;
int valS = 90;
byte Teamkleur[3];

Servo servoT;

int it = 0;
byte testsarr[10];

void setup() {
  // put your setup code here, to run once:
  int i = 0;

  Serial.begin(9600);
  //PS4.begin("5c:96:56:b2:fb:c6");
  //PS4.begin("80:ea:23:1b:fc:e7");

  
  PS4.begin(MAC_PS4);

  servoT.attach(servopin);

  initMotors(0);

  #if DEBUG > 0
  Serial.println("Waiting for controller:");
  #endif

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
  #if DEBUG > 0
  Serial.println("Controller detected:\n");
  #endif

  //initSPI();  // Initialise SPI
  i = 1;
  
  #if DEBUG > 0
  Serial.println("Starting Serial");
  #endif
  
  int state = 0;

  byte result = 0;
  
  delay(100);

  // start a state diagram for start of the SPI communication
  // This can be blocking to allow Willem to start correctly
  //  
  #if DEBUG > 0
  while (state != 5)
  {
    state = Logiclayer_Startup_Serial(state);
  }
  #endif
  #if DEBUG > 0
  Serial.println("State machine done!");
  #endif
  
  for (i = 0; i<sizeof(Teamkleur); i++)
  {
    Teamkleur[i] = My_Serial_dataIn[i+1];
  }

  Logiclayer_set_colour(Teamkleur);
  // Functie om de kleur van de esp32 te veranderen naar de corresponderende kleur

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
  // byte tests = 3;
  // Serial.write(tests);
  // byte tests1 = 0;
  // //byte testsarr[5];
  // if(Serial.available() > 0){
  //   tests1 = Serial.read();
  //   //Serial.println(tests1);
  //   //Serial.print("\n");
  //   if(tests1==10){
  //      //Serial.print("received value: ");
  //      //Serial.println(testsarr[0]);
  //      for(int ic = 0;ic<10;ic++){
  //        testsarr[ic]=0;
  //      }
  //      it = 0;
  //   }
  //   else{
  //     //Serial.println(tests1);
  //     //Serial.print("ontvangen: ");
  //     //Serial.println(tests1);
  //     testsarr[it] = tests1;
  //     //Serial.print("array waarde: ");
  //     //Serial.println(testsarr[it]);
  //     //Serial.print("it: ");
  //     //Serial.println(it);
  //     //Serial.println(testsarr[it]);
  //     it = it+1;
  //   }
  //   if(testsarr[0]>0){
  //     Serial.println(testsarr[0]);
  //   }
  //   //Serial.print("Receivewaarde: ");
  //   //char testp = (char) tests1;
  //   //tests1.trim();
  //   //int newl = "\n";
  //   //Serial.println(tests1);
  //   delay(500);
  // }
}

void Task1code( void * parameter) // Taken voor core 0
{
  // Setup
  byte* Flag_SPI = nullptr; 
  byte data[3] = {0,0,0};

  
  InitTimerInterrupt(PRESCALER, TIMERTICKS); // Init Timer interrupt returns a flag pointer 

  byte junk = 0;

  while(1) 
  {
    /*
       Gedachte kots
       Deze functie is om te vragen voor het schieten
       Als er gevraagt is wil je ongeveer een seconde wachten?
       Dus eerst kijk je naar de millis nadat het gevraagt is
       en dan om de zoveel tijd vragen hoe lang het nog duurt bijvoorbeeld 100 ms


    */

  // UI_layer_Shoot();
   if (My_Flag_SPI != 0 && Shoot == 0xAA)
   {
    Serial.println("Bang!");
    Flag_SPI = 0;
    Shoot = 0;
   }
  // String testd = Serial.readString();
  // Serial.print(testd);
    delay(100);
  }
}

void Task2code( void * parameter) // Taken voor core 1
{
  // Setup
  struct PS4 PS4InputsMain;
  
 
  int i = 5;
  

  
  while(1)
  {
    PS4InputsMain = IO_Layer_Besturing();
    PS4InputsMain = Logiclayer_Besturing_Data(PS4InputsMain);
    
    updateMotor(motorLinks, PS4InputsMain.MotordataLinks);
    updateMotor(motorRechts, PS4InputsMain.MotordataRechts);

    servodirection(PS4InputsMain.Rechterjoystick_x);
    

    // if (PS4.R2Value() > 20)
    // {
    //   PS4.setRumble(PS4.R2Value(), PS4.R2Value());
    //   PS4.sendToController();
    // }


    // Gedachte kots
    // Deze functie is om te vragen
    if (PS4InputsMain.Cirkelknop == true) // Zet een timer neer
    {
      //digitalWrite(12, HIGH);
      Shoot = 0xAA;
      Logiclayer_Serial_CMD_NO_DATA(SHOOT);
      //UI_layer_Shoot();
    }
    else
    {
      //digitalWrite(12, LOW);
    }
    
    delay(50);
  }
}

void Function_Print_Serial_output(byte CMD, byte data[5] )
{
  Serial.println();
  Serial.print("Data out:");
  Serial.print(" ");
  Serial.print(CMD, DEC);
  Serial.print(", ");
  Serial.print(data[0], DEC);
  Serial.print(", ");
  Serial.print(data[1], DEC);
  Serial.print(", ");
  Serial.print(data[2], DEC);
  Serial.println();

}

void Function_Print_Serial_input(int state)
{
  Serial.print("Data in:");
  Serial.print("State:");
  Serial.print(state, DEC);
  Serial.print(" \t\t");
  Serial.print(My_Serial_dataIn[0], DEC);
  Serial.print(", ");
  Serial.print(My_Serial_dataIn[1], DEC);
  Serial.print(", ");
  Serial.print(My_Serial_dataIn[2], DEC);
  Serial.print(", ");
  Serial.print(My_Serial_dataIn[3], DEC);
  Serial.println();
}

void servodirection(signed char Direction)
{
  if (Direction > 0 + STICKDRIFT && valS<=180)
  {
    valS = valS+1;
    servoT.write(valS);
  }
  else if (Direction < 0 - STICKDRIFT && valS>=0)
  {
    valS = valS-1;
    servoT.write(valS);
  }
  else if (Direction > 0 - STICKDRIFT && Direction < 0 + STICKDRIFT)
  {
    valS = valS;
    servoT.write(valS);
  }
  else
  {
    valS = valS;
    servoT.write(valS);
  }
}