#include "main.h"

void setup(){
  Serial.begin(115200);
  Serial2.begin(6900, SERIAL_8N1);

  /* Init Wifi */
  Serial.println("Init wifi");
  init_wifi();
  Serial.println("Wifi initialised");

  /* Init Camera */
  // Serial.println("going to init camera");
  // init_camera();
  // Serial.println("done init camera");

  delay(2000);

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
    start = WaitForMessage;
    Serial.print(".");
  }
}

void loop(){
  
  bool done = 0;
  while(!done){
    done = Gamestate();
    delay(1000);
  }

  // FSM(state);

  // delay(1000);

  // even denken als hij hieruit komt
}
