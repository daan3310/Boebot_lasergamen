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

  /* Enter idle mode, wait for wakeup from pi */
  bool start = 0;
  while(!start){
    start = WaitForMessage();
    Serial.print(".");
  }
}

uint8_t pinState = 0;
uint8_t lastPinState = 0;
void loop(){  
  bool done = 0;
  while(!done) {
    Serial.println("Send a photo");
    sendPhoto();
    Serial.println("main loop");
    done = Gamestate("/gamestate", "/00:11:22:AA:BB:CC");
    delay(1000);
    // done = 0;
  }

  while(1) {
    Serial.println("inf loop");
    delay(1000);
    if(WaitForMessage){
      break;
    }
    updateFSM();
  }
}


