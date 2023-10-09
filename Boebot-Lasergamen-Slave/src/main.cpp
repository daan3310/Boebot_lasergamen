#include "main.h"

struct PS4 PS4InputsMain;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  PS4.begin("5c:96:56:b2:fb:c6");
  initMotors(0);
}

void loop() {
  // put your main code here, to run repeatedly:
    PS4InputsMain = IO_Layer_Besturing();
    Logiclayer_Besturing_Data(PS4InputsMain);
    
    
    delay(10);
  }

