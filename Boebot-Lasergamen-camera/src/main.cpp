#include "main.h"

void setup() {
  Serial.begin(115200);
  Serial.println("going to init slave spi");

  init_slave_spi();

  Serial.println("done init slave spi");

  // init_camera();
}

DMA_ATTR char mysendbuf[8] = {0};
DMA_ATTR char myreceivebuf[8] = {0};

void loop() {
  // Serial.println("entered loop");
  // mysendbuf[0] = 69;
  // mysendbuf[1] = 70;
  // mysendbuf[2] = 71;
  // mysendbuf[3] = 72;
  // mysendbuf[4] = 73;

  // myreceivebuf[0] = 0;
  // myreceivebuf[1] = 0;
  // myreceivebuf[2] = 0;
  // myreceivebuf[3] = 0;
  // myreceivebuf[4] = 0;

  // Serial.println("going to transmit");
  // blocking_transmit_slave_spi(mysendbuf, myreceivebuf, 8*5);
  // Serial.print("flag: ");
  // Serial.println(my_post_trans_cb_flag);
  // my_post_trans_cb_flag = 0;
  
  // Serial.print("TxBuffer: ");
  // Serial.println(mysendbuf);

  // Serial.print("RxBuffer: 0b");
  // for(uint8_t counter = 0; 4 > counter; counter++)
  // {
  //   Serial.print(myreceivebuf[counter], BIN);
  //   Serial.print("\t0b");
  // }
  // Serial.println(myreceivebuf[4], BIN);
  // Serial.print("RxBuffer: 0x");
  // for(uint8_t counter = 0; 4 > counter; counter++)
  // {
  //   Serial.print(myreceivebuf[counter], HEX);
  //   Serial.print("\t0x");
  // }
  // Serial.println(myreceivebuf[4], HEX);
  // Serial.println();
  Serial.print("currentState: ");
  Serial.println(currentState);
  
  uint8_t counter = 0;

  Serial.print("TxBuffer: 0x");
  for(counter = 0; 3 > counter; counter++)
  {
    Serial.print(sendbuf[counter], HEX);
    Serial.print("\t0x");
  }
  Serial.println(sendbuf[3], HEX);
  
  Serial.print("RxBuffer: 0x");
  for(counter = 0; 3 > counter; counter++)
  {
    Serial.print(receivebuf[counter], HEX);
    Serial.print("\t0x");
  }
  Serial.println(receivebuf[3], HEX);
  Serial.println();

  updateFSM();

}