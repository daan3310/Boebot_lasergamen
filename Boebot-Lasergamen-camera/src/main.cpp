#include "main.h"

void setup(){
  Serial.begin(115200);
  Serial2.begin(6900, SERIAL_8N1);

  //might cause big issue:
  Serial.println("Init wifi");
  init_wifi();
  Serial.println("Wifi initialised");

  //might cause big issue:
  // Serial.println("going to init camera");
  // init_camera();
  // Serial.println("done init camera");

  // test
  // while(1) {
  //   // sendPhoto();
    // SendHTTPmessage("dit is een bericht.");
  //   delay(100);
  // }

  delay(3000);

  //Connect to http game server pi
  Serial.println("Init game");
  init_game();
  Serial.println("Game initialised");

  // vervang dit met seriële communicatie UART.
  // Serial.println("going to init slave spi");
  // init_slave_spi();
  // Serial.println("done init slave spi");

  while(1){
    Serial.println("wait for message");
    String message = WaitForMessage();
    Serial.println(message);
    delay(1);
  }

  bool start = 0;

  while(!start){
    if(WaitForMessage() == "start"){
      start = 1;
    }
    else{
      Serial.println(".");
    }
  }

  pinMode(GPIO_CS, INPUT);
}

uint8_t pinState = 0;
uint8_t lastPinState = 0;
void loop(){
  /*
  pinState = digitalRead(GPIO_CS);
  if((0 == lastPinState) & (1 == pinState)){
    sendPhoto();
  }
  lastPinState = pinState;
  */

  String message = WaitForMessage();
  Serial.println(message);
  delay(1);

  // FSM(state);

  // delay(1000);

  //even denken als hij hieruit komt
}
