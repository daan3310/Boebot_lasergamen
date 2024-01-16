#include <Arduino.h>

#include "driver/spi_slave.h"

#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

#define DEBUG

const char* ssid = "Leaphy Lasergame!";
const char* password = "Leaphydebug1!";

String serverName = "192.168.0.102";   
//String serverName = "example.com";   

String serverPath = "/upload";  // Flask upload route

const int serverPort = 5000;

const int timerInterval = 10000;    // time (milliseconds) between each HTTP POST image
unsigned long previousMillis = 0;   // last time image was sent

WiFiClient client;

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

String sendPhoto();

#include <stdio.h>

char sendbuf[128] = {0};
spi_slave_transaction_t t;
int i;

void setup() {
  Serial.begin(115200);

  // Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=13,
        .miso_io_num=16,
        .sclk_io_num=14,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    // Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .spics_io_num=15,
        .flags=0,
        .queue_size=3,
        .mode=0,
    };


    // Initialize SPI slave interface
    spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);

    // SPI variables 
    sendbuf;
    t;
    memset(&t, 0, sizeof(t));
    
    printf("Slave transmission:\n");

  // spi_bus_config_t SPI_Bus_Config;
  // SPI_Bus_Config.mosi_io_num = 13;
  // SPI_Bus_Config.miso_io_num = 16;
  // SPI_Bus_Config.sclk_io_num = 14;
  // SPI_Bus_Config.data0_io_num = -1;
  // SPI_Bus_Config.data1_io_num = -1;
  // SPI_Bus_Config.data2_io_num = -1;
  // SPI_Bus_Config.data3_io_num = -1;
  // SPI_Bus_Config.data4_io_num = -1;
  // SPI_Bus_Config.data5_io_num = -1;
  // SPI_Bus_Config.data6_io_num = -1;
  // SPI_Bus_Config.data7_io_num = -1;
  // // SPI_Bus_Config.flags = -1;
  // // SPI_Bus_Config.intr_flags = -1;
  // SPI_Bus_Config.max_transfer_sz = 4092;
  // SPI_Bus_Config.quadhd_io_num = -1;
  // SPI_Bus_Config.quadwp_io_num = -1;

  // spi_slave_interface_config_t SPI_Slave_Interface_Config;
  // SPI_Slave_Interface_Config.spics_io_num = 15; // chip select
  // // SPI_Slave_Interface_Config.flags = -1;
  // SPI_Slave_Interface_Config.mode = 0;
  // // SPI_Slave_Interface_Config.post_setup_cb = -1;
  // // SPI_Slave_Interface_Config.post_trans_cb = -1;
  // SPI_Slave_Interface_Config.queue_size = 4;


  // // spi_slave_initialize(spi_host_device_t host, const spi_bus_config_t *bus_config, const spi_slave_interface_config_t *slave_config, spi_dma_chan_t dma_chan)
  // Serial.print("spi_slave_initialize returns: ");
  // Serial.println(spi_slave_initialize(HSPI_HOST, &SPI_Bus_Config, &SPI_Slave_Interface_Config, SPI_DMA_CH_AUTO));

  

  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  // Serial.begin(115200);

  // WiFi.mode(WIFI_STA);
  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  // WiFi.begin(ssid, password);  
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print(".");
  //   delay(500);
  // }
  // Serial.println();
  // Serial.print("ESP32-CAM IP Address: ");
  // Serial.println(WiFi.localIP());

  // camera_config_t config;
  // config.ledc_channel = LEDC_CHANNEL_0;
  // config.ledc_timer = LEDC_TIMER_0;
  // config.pin_d0 = Y2_GPIO_NUM;
  // config.pin_d1 = Y3_GPIO_NUM;
  // config.pin_d2 = Y4_GPIO_NUM;
  // config.pin_d3 = Y5_GPIO_NUM;
  // config.pin_d4 = Y6_GPIO_NUM;
  // config.pin_d5 = Y7_GPIO_NUM;
  // config.pin_d6 = Y8_GPIO_NUM;
  // config.pin_d7 = Y9_GPIO_NUM;
  // config.pin_xclk = XCLK_GPIO_NUM;
  // config.pin_pclk = PCLK_GPIO_NUM;
  // config.pin_vsync = VSYNC_GPIO_NUM;
  // config.pin_href = HREF_GPIO_NUM;
  // config.pin_sscb_sda = SIOD_GPIO_NUM;
  // config.pin_sscb_scl = SIOC_GPIO_NUM;
  // config.pin_pwdn = PWDN_GPIO_NUM;
  // config.pin_reset = RESET_GPIO_NUM;
  // config.xclk_freq_hz = 20000000;
  // config.pixel_format = PIXFORMAT_JPEG;

  // // init with high specs to pre-allocate larger buffers
  // if(psramFound()){
  //   config.frame_size = FRAMESIZE_SVGA;
  //   config.jpeg_quality = 10;  //0-63 lower number means higher quality
  //   config.fb_count = 2;
  // } else {
  //   config.frame_size = FRAMESIZE_CIF;
  //   config.jpeg_quality = 12;  //0-63 lower number means higher quality
  //   config.fb_count = 1;
  // }
  
  // // camera init
  // esp_err_t err = esp_camera_init(&config);
  // if (err != ESP_OK) {
  //   Serial.printf("Camera init failed with error 0x%x", err);
  //   delay(1000);
  //   ESP.restart();
  // }

  // sendPhoto(); 
}

void loop() {

  // snprintf(sendbuf, sizeof(sendbuf), "Sent by slave - %d", i);
  snprintf(sendbuf, sizeof(sendbuf), "%d", i);
      t.length = sizeof(sendbuf) * 8;
      t.tx_buffer = sendbuf;
      spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
      printf("Transmitted: %s\n", sendbuf);
      delay(1000);
      i++;

  // WORD_ALIGNED_ATTR uint8_t rx_buffer[4];
  // WORD_ALIGNED_ATTR uint8_t tx_buffer[4];

  // // rx_buffer[0] = 0xAF;
  // // rx_buffer[1] = 0xFA;
  // // rx_buffer[2] = 0xAF;
  // // rx_buffer[3] = 0xFA;

  // tx_buffer[0] = 0xAA;
  // tx_buffer[1] = 0xAA;
  // tx_buffer[2] = 0xAA;
  // tx_buffer[3] = 0xAA;

  // spi_slave_transaction_t spi_slave_transaction;
  // spi_slave_transaction.rx_buffer = &rx_buffer;
  // spi_slave_transaction.tx_buffer = &tx_buffer;
  // spi_slave_transaction.length = 8;
  // spi_slave_transaction.trans_len = 8;

  // Serial.print("Before:\nRX:");

  // Serial.print(rx_buffer[0]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[1]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[2]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[3]);
  // Serial.print("\nTX:");

  // Serial.print(tx_buffer[0]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[1]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[2]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[3]);
  // Serial.print("\n\n");

  // // spi_slave_queue_trans(spi_host_device_t host, const spi_slave_transaction_t *trans_desc, TickType_t ticks_to_wait)
  // Serial.print("spi_slave_queue_trans returns: ");
  // Serial.println(spi_slave_queue_trans(HSPI_HOST, &spi_slave_transaction, portMAX_DELAY));

  // spi_slave_transaction_t *spi_slave_transaction_pointer;

  // delay(200);

  // // spi_slave_get_trans_result(spi_host_device_t host, spi_slave_transaction_t **trans_desc, TickType_t ticks_to_wait)
  // Serial.print("spi_slave_get_trans_result returns: ");
  // Serial.println(spi_slave_get_trans_result(HSPI_HOST, &spi_slave_transaction_pointer, portMAX_DELAY));

  // Serial.print("After:\nRX:");

  // Serial.print(rx_buffer[0]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[1]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[2]);
  // Serial.print(", ");
  // Serial.print(rx_buffer[3]);
  // Serial.print("\nTX:");

  // Serial.print(tx_buffer[0]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[1]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[2]);
  // Serial.print(", ");
  // Serial.print(tx_buffer[3]);
  // Serial.print("\n\n");

  
//   unsigned long currentMillis = millis();
//   if (currentMillis - previousMillis >= timerInterval) 
//   {
//     currentMillis = millis();
//     Serial.print("Time before:");
//     Serial.println(currentMillis);
//     sendPhoto();
//     currentMillis = millis();
//     Serial.print("Time after:");
//     Serial.println(currentMillis);
//     previousMillis = currentMillis;
//   }
// }

// String sendPhoto() {
//   String getAll;
//   String getBody;

//   camera_fb_t *fb = NULL;
//   fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     delay(1000);
//     ESP.restart();
//   }
//   #ifdef DEBUG  
//   Serial.println("Connecting to server: " + serverName);
//   #endif 

//   if (client.connect(serverName.c_str(), serverPort)) {
    
//     #ifdef DEBUG
//     Serial.println("Connection successful!");
//     #endif

//     String head = "--ESP32\r\nContent-Disposition: form-data; name=\"image\"; filename=\"1\"\r\nContent-Type: image\r\n\r\n";
//     String tail = "\r\n--ESP32--\r\n";

//     uint16_t imageLen = fb->len;
//     uint16_t extraLen = head.length() + tail.length();
//     uint16_t totalLen = imageLen + extraLen;
    
//     #ifdef DEBUG
//     Serial.println("Sending HTTP POST request...");
//     #endif

//     client.println("POST " + serverPath + " HTTP/1.1");
//     client.println("Host: " + serverName);
//     client.println("Content-Length: " + String(totalLen));
//     client.println("Content-Type: multipart/form-data; boundary=ESP32");
//     client.println();
//     client.print(head);

//     uint8_t *fbBuf = fb->buf;
//     size_t fbLen = fb->len;

//     for (size_t n = 0; n < fbLen; n = n + 1024) {
//       if (n + 1024 < fbLen) {
//         client.write(fbBuf, 1024);
//         fbBuf += 1024;
//       } else if (fbLen % 1024 > 0) {
//         size_t remainder = fbLen % 1024;
//         client.write(fbBuf, remainder);
//       }
//     }
//     client.print(tail);

//     Serial.println("Waiting for server response...");

//     esp_camera_fb_return(fb);

//     //boolean state = false;

//     // while ((startTimer + timeoutTimer) > millis()) {
//     //   Serial.print(".");
//     //   delay(10);
//     //   while (client.available()) {
//     //     char c = client.read();
//     //     if (c == '\n') {
//     //       if (getAll.length() == 0) {
//     //         state = true;
//     //       }
//     //       getAll = "";
//     //     } else if (c != '\r') {
//     //       getAll += String(c);
//     //     }
//     //     if (state == true) {
//     //       getBody += String(c);
//     //     }
//     //     startTimer = millis();
//     //   }
//     //   if (getBody.length() > 0) {
//     //     break;
//     //   }
//     // }
//     //Serial.println();
//     client.stop();
//     //Serial.println(getBody);
//   } 
//   else 
//   {
    
//     #ifdef DEBUG
//     Serial.print("Connection to ");
//     Serial.print(serverName);
//     getBody = "Connection failed.";
//     Serial.println(getBody);
//     #endif

//   }
//   return getBody;
}