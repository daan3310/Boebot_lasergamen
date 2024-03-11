#ifndef IO_LAYER_H
#define IO_LAYER_H

#include "main.h"

#define USE_SPI 1
#ifdef USE_SPI

#include "driver/spi_slave.h"

#define MAP_SPI_PINS 1
#ifdef MAP_SPI_PINS
#define GPIO_MOSI           13
#define GPIO_MISO           16
#define GPIO_SCLK           14
#define GPIO_CS             15
#endif

extern DMA_ATTR char sendbuf[6];
extern DMA_ATTR char receivebuf[6];
extern spi_slave_transaction_t t;
// extern uint8_t i;
// extern uint8_t r;
extern uint8_t my_post_trans_cb_flag;

void my_post_trans_cb(spi_slave_transaction_t *trans);
void init_game(void);
bool connect_pi(void);

esp_err_t init_slave_spi();

esp_err_t blocking_transmit_slave_spi(void* TxBuf, void*RxBuf, uint Length_in_bits);

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
// // extern String serverName;   

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



esp_err_t init_camera();

String sendPhoto();

unsigned long benchMark_sendPhoto();
#endif
//*/

#endif 
