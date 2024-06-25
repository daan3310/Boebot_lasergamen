#include "IO-layer.h"

#define SERIAL_BAUD_RATE 115200  // Change baud rate as needed

Servo myMotorTurret;
SPIClass * vspi = NULL;

byte My_Flag_SPI;

char** My_Serial_dataIn;
byte My_Serial_dataIn;



void IRAM_ATTR Timer0_ISR()
{
  My_Flag_SPI = 1;
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
            // motorPower = 1024;
            // delay(1);
            motorPower = -1024;
            //returnValue = updateMotor_exceeded_lower_bound_of_motorPower;
        }
        else
        {
            // motorPower = -1024;
            // delay(1);
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

// byte initSerial() {
//   //Serial.begin(SERIAL_BAUD_RATE);  // Initialize serial communication
//   Serial.begin(9600);
// }

byte serial_send_command(char* cmd, char** inputArr) {
byte serial_send_command(byte cmd) {
  #if DEBUG > 0
  Function_Print_Serial_output(cmd);
  Function_Print_Serial_output(cmd);
  #endif

  char* dataIn;
  //char* dataIn;
  // Send data out through serial
  Serial.write(cmd);
  Serial1.write(cmd);

  // Read incoming data if needed
  if (Serial.available()) {
    for (int i = 0; i < 4; i++) {
      String dataRec = Serial.readStringUntil('.'); 
      strcpy(dataIn, dataRec.c_str());
      Serial.print(dataIn[i]);
      Serial.print(", ");
    }
    char *token;
	  token = strtok(dataIn, ",");
	  int i;
	  int p = 0;
	  while(token != NULL){
		  for(i=0; i<strlen(token); i++){
			  inputArr[p][i] = token[i];
		  }
		  p++;
		  token = strtok(NULL, ",");
	  }
	  p = 0;
  }
  // Copy received data to your buffer
  for (int i = 0; i < 4; i++) {
    strcpy(My_Serial_dataIn[i], inputArr[i]);
    //My_Serial_dataIn[i] = inputArr[i];
  if (Serial1.available() > 0) {
      byte dataRec = Serial1.read(); 
    //   int dataRec1 = (int)dataRec;
    //   Serial.print("De receive data: ");
    //   Serial.println(dataRec);
      My_Serial_dataIn = dataRec;
  }

  //Function_Print_Serial_input(state);

  Serial.print("De receive data: ");
  Serial.println(My_Serial_dataIn);
  return 0;
}

void LedLevens(){
  byte data[DATALENGTH - 1] = {0};  // Initialize the data array with 0s
  //serial_send_command(GAMESTATE, data);
  CRGB leds_levens[NUM_LEDS_LEVENS];
  FastLED.addLeds<WS2812, LED_PIN_LEVENS, GRB>(leds_levens, NUM_LEDS_LEVENS);
  int lives = My_Serial_dataIn[0];
   if (lives > NUM_LEDS_LEVENS) {
        lives = NUM_LEDS_LEVENS;
    }
    
    // Turn on LEDs for the lives
    for (int i = 0; i < lives; i++) {
        leds_levens[i] = CRGB::Red; // Set the color of the lives LEDs, e.g., red
    }
    
    // Turn off remaining LEDs
    for (int i = lives; i < NUM_LEDS_UNDERGLOW; i++) {
        leds_levens[i] = CRGB::Black; // Set the color to black (off)
    }
    
    FastLED.show(); // Update the LED strip to display changes
}


void InitTimerInterrupt(uint Prescaler, uint TimerTicks)
{
    hw_timer_t *Timer0_Cfg = NULL;

    Timer0_Cfg = timerBegin(0, Prescaler, true); 
    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
    timerAlarmWrite(Timer0_Cfg, TimerTicks, true); 
    timerAlarmEnable(Timer0_Cfg);
}
void InitLedStrip(){
 byte data[DATALENGTH - 1] = {0};  // Initialize the data array with 0s
  CRGB leds_underglow[NUM_LEDS_UNDERGLOW];
  
  FastLED.addLeds<WS2812, LED_PIN_UNDERGLOW, GRB>(leds_underglow, NUM_LEDS_UNDERGLOW  );
  serial_send_command(TEAMCOLOUR, data);
  CRGB Teamkleur = CRGB(My_Serial_dataIn[0], My_Serial_dataIn[1], My_Serial_dataIn[2]);
  fill_solid(leds_underglow, NUM_LEDS_UNDERGLOW, Teamkleur);
  FastLED.show();  
}


void InitStepperMotor()
{
    
}

