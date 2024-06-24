#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"

#define USE_SPI 1
#ifdef USE_SPI

#include "driver/spi_slave.h"
#include <HardwareSerial.h>
#include <string.h>

#define MAP_SPI_PINS 1

#ifdef MAP_SPI_PINS
#define GPIO_MOSI           13
#define GPIO_MISO           16
#define GPIO_SCLK           14
#define GPIO_CS             15
#endif

#define GPIO_TX 1
#define GPIO_RX 3

// extern DMA_ATTR char sendbuf[4];
// extern DMA_ATTR char receivebuf[4];
extern spi_slave_transaction_t t;
extern uint8_t my_post_trans_cb_flag;

void my_post_trans_cb(spi_slave_transaction_t *trans);

esp_err_t blocking_transmit_slave_serial(char* TxBuf, char*RxBuf);

#endif


#define USE_WIFI 1
#ifdef USE_WIFI

#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <HTTPClient.h>

// extern const char* ssid;
// extern const char* password;
// extern String serverName;   
// extern String serverName;   
// extern String serverPath;  // Flask upload route
// extern const int serverPort;
// extern WiFiClient client;

IPAddress init_wifi();

#endif
//*/



#define USE_CAMERA 1
#ifdef USE_CAMERA

#include "esp_camera.h"

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define MAC_ADDRESS_DEF "03:11:22:AA:BB:CC"

esp_err_t init_camera();
bool connect_pi(String server_path,String address);
void init_game(void);
bool Gamestate(String server_path,String address);
int WaitForMessage(void);
String sendPhoto();

unsigned long benchMark_sendPhoto();
#endif
//*/

#endif 
