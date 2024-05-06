#include "main.h"

void setup() {
  Serial.begin(9600);
  //might cause big issue:
  // Serial.println("going to init wifi");
  // init_wifi();
  // Serial.println("done init wifi");

  //might cause big issue:
  // Serial.println("going to init camera");
  // init_camera();
  // Serial.println("done init camera");

  // Serial.println("going to init slave spi");
  // init_slave_spi();
  // Serial.println("done init slave spi");
  
  //pinMode(GPIO_CS, INPUT);
}

uint8_t pinState = 0;
uint8_t lastPinState = 0;
void loop() {
  // Serial.print("currentState: ");
  // Serial.println(currentState);
  
  // uint8_t counter = 0;

  // Serial.print("TxBuffer: 0x");
  // for(counter = 0; 3 > counter; counter++)
  // {
  //   Serial.print(sendbuf[counter], HEX);
  //   Serial.print("\t0x");
  // }
  // Serial.println(sendbuf[3], HEX);
  
  // Serial.print("RxBuffer: 0x");
  // for(counter = 0; 3 > counter; counter++)
  // {
  //   Serial.print(receivebuf[counter], HEX);
  //   Serial.print("\t0x");
  // }
  // Serial.println(receivebuf[3], HEX);
  // Serial.println();

  // Serial.println("Begintest");
  // updateFSM();
  //sendPhoto();
  // delay(1000);
  byte test1 = 0;
  if(Serial.available()>0){
    test1 = Serial.read();
    while(Serial.available()>0){
      Serial.read();
    }
    Serial.write(test1);
    //Serial.write("\n");
  }
  //Serial.write(54);
  //even denken als hij hieruit komt
}
