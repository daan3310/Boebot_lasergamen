#include "main.h"

#include <WiFiUdp.h>

void setup() {
  /* setup serial communication */
  Serial.begin(9600);

  /* might cause big issue: */
  //Serial.println("going to init wifi");
  init_wifi();
  // Serial.println("done init wifi");

  /* might cause big issue:*/
  // Serial.println("going to init camera");
  init_camera();
  // Serial.println("done init camera");

  /* Connect to http game server pi  */
  // Serial.println("Init game");
  init_game();
  // Serial.println("Game initialised");

  /* Enter idle mode, wait for wakeup from pi */
  int start = 0;
  // Serial.println("Enter idle mode.");
  while(start != 1)
  {
    updateFSM();
    start = WaitForMessage();
  }
  // Serial.println("Exit idle mode.");

  /* request gamestate */
  int error = Gamestate("/gamestate/", MAC_ADDRESS_DEF);
}

void loop() {  
  MessageFSM(WaitForMessage());
  updateFSM();
}


