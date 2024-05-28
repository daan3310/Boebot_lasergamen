#include "main.h"

void setup(){
  Serial.begin(115200);
  // Serial2.begin(6900, SERIAL_8N1);

  /* Init Wifi */
  Serial.println("Init wifi");
  init_wifi();
  Serial.println("Wifi initialised");

  /* Init Camera <-- camera errort, problemen met Serial2??? */
  Serial.println("going to init camera");
  init_camera();
  Serial.println("done init camera");

  /* Connect to http game server pi  */
  Serial.println("Init game");
  init_game();
  Serial.println("Game initialised");

  /* Init SPI ,vervang dit met seriële communicatie UART. */ 
  // Serial.println("going to init slave spi");
  // init_slave_spi();
  // Serial.println("done init slave spi");

  /* Enter idle mode, wait for wakeup from pi */
  bool start = 0;
  while(!start){
    start = WaitForMessage();
    Serial.print(".");
  }
}

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

  // delay(1000);

  // even denken als hij hieruit komt
}


