#include "IO-layer.h"

DMA_ATTR char sendbuf[6] = {0};
DMA_ATTR char receivebuf[6] = {0};
spi_slave_transaction_t t;
// uint8_t i;
// uint8_t r;
uint8_t my_post_trans_cb_flag = 0;


#ifdef USE_SPI

void my_post_trans_cb(spi_slave_transaction_t *trans){
  my_post_trans_cb_flag = 1;
  // Serial.println("my_post_trans_cb");
}

esp_err_t init_slave_spi(){
    // Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    // Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .spics_io_num=GPIO_CS,
        .flags=0,
        .queue_size=5,
        .mode=3,
        // .post_setup_cb,
        .post_trans_cb = my_post_trans_cb,
        
    };

    // Initialize SPI slave interface
    esp_err_t error = spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    // clear spi_slave_transaction_t
    memset(&t, 0, sizeof(t));
    return error;
}

esp_err_t blocking_transmit_slave_spi(void* TxBuf, void*RxBuf, uint Length_in_bits){
  t.length = Length_in_bits;
  // i = 170;
  t.tx_buffer = TxBuf;
  // r = 0;
  t.rx_buffer = RxBuf;
  
  return spi_slave_transmit(HSPI_HOST, &t, 1000); // portMAX_DELAY
  
  // return ESP_OK;
}

esp_err_t non_blocking_queue_transaction_slave_spi(void* TxBuf, void*RxBuf, uint Length_in_bits){
  t.length = Length_in_bits;
  t.tx_buffer = TxBuf;
  t.rx_buffer = RxBuf;

  return spi_slave_queue_trans(HSPI_HOST, &t, portMAX_DELAY);
  
  // return spi_slave_get_trans_result(HSPI_HOST, ppt, portMAX_DELAY);
  // printf("Transmitted: %u\n", i);
  // printf("Received:    %u\n\n", r);
  //r = *(pt->rx_buffer);
  //printf("Received?:   %u\n\n", r);
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

#endif

#ifdef USE_WIFI
const char* ssid     = "Leaphy Lasergame!";
const char* password = "Leaphydebug1!";
String serverName    = "192.168.0.102";   
String serverPath    = "/upload";  // Flask upload route
const int serverPort = 5000;
WiFiClient client;

void init_game(void){
  bool connected = 0;
  while(!connected){
    if (connect_pi()==1){
      connected = 1;
    }
    else{
      Serial.println("Connecting ..");
      delay(500);
    }
  }
}

/* Connect to the HTTP server on the Rpi by sending ESP MAC-ADDRESS */ 
bool connect_pi(void){
  HTTPClient http;

  http.begin("http://" + String(serverName) + serverPath);
  http.addHeader("Content-Type", "Mac address");
  String MAC_ADDRESS = "16";
  int address_length = MAC_ADDRESS.length();
  int totalLen = address_length;

  int httpResponseCode = http.POST(MAC_ADDRESS);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Read the JSON response from the server
    DynamicJsonDocument jsonDoc(1024);  // Adjust the size based on your JSON response
    deserializeJson(jsonDoc, http.getString());

    // Extract data from the JSON response
    String acknowledgment = jsonDoc["acknowledgment"];
    Serial.println("Acknowledgment: " + acknowledgment);
  } else {
    Serial.print("Error in HTTP request. Code: ");
    Serial.println(httpResponseCode);
  }
  
  return 1;
}

IPAddress init_wifi(){
    Serial.println("going to init wifi");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout

    WiFi.mode(WIFI_STA);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
    }
    Serial.println();
    Serial.print("ESP32-CAM IP Address: ");
    Serial.println(WiFi.localIP());

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); // enable brownout

    Serial.println("done init wifi");
    return WiFi.localIP();
}

#endif

#ifdef USE_CAMERA
String sendPhoto(){
  String getAll;
  String getBody;

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb){
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)){

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
      if (n + 1024 < fbLen){
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen % 1024 > 0){
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
  else{

    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    getBody = "Connection failed.";
    Serial.println(getBody);
    #endif

  }
  return getBody;
}

unsigned long benchMark_sendPhoto(){
    unsigned long lastMillis = millis();
    
    lastMillis = millis();
    
    sendPhoto();
    return millis() - lastMillis;

    // if (currentMillis - previousMillis >= timerInterval) 
    // {

    // }
}

esp_err_t init_camera(){
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
    } 
    else{
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
    }
  
    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK){
    Serial.printf("Camera init failed with error 0x%x", err);
    }
    return err;
}

#endif
