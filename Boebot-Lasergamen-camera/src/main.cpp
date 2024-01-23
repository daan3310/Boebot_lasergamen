#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

//#define DEBUG

const char* ssid = "Leaphy Lasergame!";
const char* password = "Leaphydebug1!";

String serverName = "192.168.0.102";   
//String serverName = "example.com";   

String serverPath = "/upload";  // Flask upload route

const int serverPort = 5000;

const int timerInterval = 1000;    // time (milliseconds) between each HTTP POST image
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

void sendPhoto();

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());

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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 64;  //0-63 lower number means higher quality  // maybe this one?
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 32;  //0-63 lower number means higher quality  // 12 121ms    // 32 100ms // 64 50ms
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //sendPhoto(); 
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= timerInterval) 
  {
    unsigned long benchmarkMillisPrev = millis();
    sendPhoto();
    unsigned long benchmarkMillisCurr = millis();

    //Serial.print("Time to send:");
    Serial.println(benchmarkMillisCurr - benchmarkMillisPrev);

    previousMillis = currentMillis;
  }
}

char header[] = "--ESP32\r\nContent-Disposition: form-data; name=\"image\"; filename=\"1\"\r\nContent-Type: image\r\n\r\n";
char footer[] = "\r\n--ESP32--\r\n";

void sendPhoto() {
  camera_fb_t *fb = esp_camera_fb_get();

  #ifdef DEBUG
  Serial.println("Connecting to server: " + serverName);
  #endif

  if (client.connect(serverName.c_str(), serverPort)) {
    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    size_t imageLen = fb->len;
    size_t extraLen = sizeof(header) + sizeof(footer) - 2; // Exclude null terminator
    size_t totalLen = imageLen + extraLen;

    #ifdef DEBUG
    Serial.println("Sending HTTP POST request...");
    #endif

    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=ESP32");
    client.println();

    client.write(header, sizeof(header) - 1);

    // Sending chunks of data to the Raspberry Pi
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;

    for (size_t n = 0; n < fbLen; ) {
      size_t chunkSize = (1024 < fbLen - n) ? 1024 : fbLen - n;
      client.write(fbBuf + n, chunkSize);
      n += chunkSize;
    }

    client.write(footer, sizeof(footer) - 1);

    #ifdef DEBUG
    Serial.println("Waiting for server response...");
    #endif

    esp_camera_fb_return(fb);

    // unsigned long timeoutEndTime = millis() + 10000;
    // while (millis() < timeoutEndTime && !client.available()) {
    //   // Wait for server response
    // }

    client.stop();
  } else {
    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    Serial.println(" failed.");
    #endif
  }
}