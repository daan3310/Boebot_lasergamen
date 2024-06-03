#include "main.h"

void setup() {
  Serial.begin(9600);

  //might cause big issue:
  Serial.println("going to init wifi");
  init_wifi();
  Serial.println("done init wifi");

  //might cause big issue:
  Serial.println("going to init camera");
  init_camera();
  Serial.println("done init camera");

  /* Connect to http game server pi  */
  Serial.println("Init game");
  init_game();
  Serial.println("Game initialised");
}

uint8_t pinState = 0;
uint8_t lastPinState = 0;
void loop() {

  updateFSM();
  
}


