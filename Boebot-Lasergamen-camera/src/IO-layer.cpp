#include "IO-layer.h"

#define SERIAL_BAUD_RATE 115200  // Change baud rate as needed

char sendbuf[4] = {0};
char receivebuf[4] = {0};

// Define callback flag and function
uint8_t my_post_trans_cb_flag = 0;

void my_post_trans_cb()
{
  my_post_trans_cb_flag = 1;
}

// void init_slave_serial() {
//   Serial.begin(SERIAL_BAUD_RATE);  // Initialize serial communication
// }

esp_err_t blocking_transmit_slave_serial(void* TxBuf, void* RxBuf, uint Length_in_bits)
{
  // Cast the buffers to char pointers
  char* txBuffer = (char*)TxBuf;
  char* rxBuffer = (char*)RxBuf;
  //Serial.println("Begin");
  // Send data out through serial
  // for(int i1 = 0; i1 < Length_in_bits / 8; i1++){
  //   Serial.println(txBuffer[i1]);
  // }
  Serial.write(txBuffer, Length_in_bits / 8);

  // Wait for incoming data
  uint32_t start_time = millis();
  while (Serial.available() <= (Length_in_bits / 8)) {
    if (millis() - start_time > 2000) {
      return ESP_ERR_TIMEOUT; // Return timeout error if not enough data received within timeout
    }
  }

  // Read incoming data
  //Serial.readBytes(rxBuffer, Length_in_bits / 8);
  for (int i = 0; i <= (Length_in_bits / 8); i++) {
    rxBuffer[i] = Serial.read();
  }
  
  return ESP_OK;
}

esp_err_t non_blocking_queue_transaction_slave_serial(void* TxBuf, void* RxBuf, uint Length_in_bits)
{
  // This function is blocking in the case of Serial communication
  return blocking_transmit_slave_serial(TxBuf, RxBuf, Length_in_bits);
}


// void* poll_transaction_complete();
// {
  
//   if(1 = my_post_trans_cb_flag)
//   {
//     spi_slave_transaction_t *pt;
//     spi_slave_transaction_t **ppt = &pt;
//     spi_slave_get_trans_result(HSPI_HOST, ppt, portMAX_DELAY);
    
//   }
//   return (void*)NULL;
// }

//#endif

#ifdef USE_WIFI
const char* ssid = "Leaphy Lasergame!";
const char* password = "Leaphydebug1!";
String serverName = "192.168.0.102";   
String serverPath = "/upload";  // Flask upload route
const int serverPort = 5000;
WiFiClient client;

IPAddress init_wifi()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout

    WiFi.mode(WIFI_STA);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    }
    Serial.println();
    Serial.print("ESP32-CAM IP Address: ");
    Serial.println(WiFi.localIP());

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); // enable brownout

    return WiFi.localIP();
}

#endif

#ifdef USE_CAMERA
String sendPhoto() {
  String getAll;
  String getBody;

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)) {

    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    String head = "--ESP32\r\nContent-Disposition: form-data; name=\"image\"; filename=\"1\"\r\nContent-Type: image\r\n\r\n";
    String tail = "\r\n--ESP32--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    #ifdef DEBUG
    Serial.println("Sending HTTP POST request...");
    #endif

    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=ESP32");
    client.println();
    client.print(head);

    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;

    for (size_t n = 0; n < fbLen; n = n + 1024) {
      if (n + 1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen % 1024 > 0) {
        size_t remainder = fbLen % 1024;
        client.write(fbBuf, remainder);
      }
    }
    client.print(tail);

    Serial.println("Waiting for server response...");

    esp_camera_fb_return(fb);

    //boolean state = false;

    // while ((startTimer + timeoutTimer) > millis()) {
    //   Serial.print(".");
    //   delay(10);
    //   while (client.available()) {
    //     char c = client.read();
    //     if (c == '\n') {
    //       if (getAll.length() == 0) {
    //         state = true;
    //       }
    //       getAll = "";
    //     } else if (c != '\r') {
    //       getAll += String(c);
    //     }
    //     if (state == true) {
    //       getBody += String(c);
    //     }
    //     startTimer = millis();
    //   }
    //   if (getBody.length() > 0) {
    //     break;
    //   }
    // }
    //Serial.println();
    client.stop();
    //Serial.println(getBody);
  } 
  else 
  {

    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    getBody = "Connection failed.";
    Serial.println(getBody);
    #endif

  }
  return getBody;
}

unsigned long benchMark_sendPhoto()
{
    unsigned long lastMillis = millis();
    
    lastMillis = millis();
    
    sendPhoto();
    return millis() - lastMillis;

    // if (currentMillis - previousMillis >= timerInterval) 
    // {

    // }
}

esp_err_t init_camera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // init with high specs to pre-allocate larger buffers
    if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
    } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
    }
  
    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    }
    return err;
}

#endif
