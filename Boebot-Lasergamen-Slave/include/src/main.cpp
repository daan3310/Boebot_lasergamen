#include "main.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  PS4.begin("5c:96:56:b2:fb:c6");
}

void loop() {
  // put your main code here, to run repeatedly:

    Logiclayer_Besturing();
    delay(1000);

  }


