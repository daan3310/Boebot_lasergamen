#include "main.h"

#include <WiFiUdp.h>

void setup() {
  /* setup serial communication */
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, GPIO_RX, GPIO_TX);

  /* Initialize the wifi: */
  init_wifi();

  /* Initialize the camera:*/
  init_camera();

  /* Connect to http game server pi  */
  init_game();

  /* Send test debug message */
  SendMessage("/ESP_DEBUG","Start setup");

  /* Enter idle mode, wait for wakeup from pi */
  int start = 0;
  while(start != 1) {
    updateFSM();
    start = WaitForMessage();
  }

  /* request gamestate */
  int error = Gamestate("/gamestate/", MAC_ADDRESS_DEF);
}

void loop() {  
  MessageFSM(WaitForMessage());
  updateFSM();
}


