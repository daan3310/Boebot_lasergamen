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

esp_err_t blocking_transmit_slave_serial(void* TxBuf, void* RxBuf, uint Length_in_bits)
{
  // Cast the buffers to char pointers
  char* txBuffer = (char*)TxBuf;
  char* rxBuffer = (char*)RxBuf;
  
  Serial.write(txBuffer, Length_in_bits / 8);

  // Wait for incoming data
  uint32_t start_time = millis();
  while (Serial.available() <= (Length_in_bits / 8)) {
    if (millis() - start_time > 2000) {
      return ESP_ERR_TIMEOUT; // Return timeout error if not enough data received within timeout
    }
  }

  // Read incoming data
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

#ifdef USE_WIFI
const char* ssid           = "Leaphy Lasergame!";
const char* password       = "Leaphydebug1!";
String serverName          = "192.168.0.102";   
String ServerPathStartup   = "/startup";  // Flask upload route (voor image) (flask library python)
String ServerPathGamestate = "/gamestate/";
String ServerPathShoot     = "/shoot";
String MAC                 = "00:11:22:AA:BB:CC";
const char* MAC_str        = "00:11:22:AA:BB:CC";
const int serverPort       = 5000;
WiFiClient client;

/**
 * @brief game init loop
 * 
 * Blijf in deze loop hangen totdat connect_pi 1 returnt. 
 * 
 * @see connect_pi() 
 */
void init_game(void){
  bool connected = 0;
  Serial.println("Connecting to HTTP server");
  while(!connected){
    connected = connect_pi(ServerPathStartup, MAC);
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
}

/**
 * @brief Connect to the host
 * 
 * Het device stuurt een bericht naar de host met het mac-address om zich aan te melden. Zodra het is aangemeld bij de host,
 * stuurt deze een acknowladgement bericht terug. 
 */
bool connect_pi(String server_path,String address) {
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)) {

    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    String head        = "--ESP32\r\nContent-Disposition: form-data; name=\"mac_address\"; filename=\"12345678\"\r\nContent-Type: mac_address\r\n\r\n";
    String tail        = "\r\n--ESP32--\r\n";
    String mac_address = address; //tijdelijk mac address, mac address nader te bepalen'

    uint16_t totalLen  = head.length() + tail.length() + mac_address.length();

    #ifdef DEBUG
    Serial.println("Sending HTTP POST request...");
    #endif

    /* Send HTTP request */ 
    client.println("POST " + server_path + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=ESP32");
    client.println();
    client.print(head);
    client.print(mac_address);
    client.print(tail);

    #ifdef DEBUG
    Serial.println("Waiting for server response...");
    #endif

    /* wait for response */ 
    while (!client.available()) {
      delay(100);
      Serial.print(".");
    }

    /* read response */
    String decoded_string;
    String input = client.readString();
    Serial.println(input);

    /* remove http overhead from input */
    int jsonStartIndex = input.indexOf("\r\n\r\n");

    if (jsonStartIndex != -1) {
      decoded_string = input.substring(jsonStartIndex + 4);
      decoded_string.trim();

      #ifdef DEBUG
      Serial.println("Extracted JSON content:");
      Serial.println(decoded_string);
      #endif
    } 
    else {
      #ifdef DEBUG
      Serial.println("JSON content not found in HTTP response.");
      #endif
      return 0;
    }

    /* parse JSON */
    DynamicJsonDocument doc(512);  //192
    DeserializationError error = deserializeJson(doc, decoded_string);

    if (error) {
      #ifdef DEBUG
      Serial.print(F("JSON parsing failed: "));
      Serial.println(error.f_str());
      #endif
      return 0;
    } 
    else {
      /* Access parsed JSON data */ 
      const char* message = doc["message"];         // "Registered MAC <MAC_ADDRESS> for IP <IP_ADDRESS>."
      const char* MAC_ADDRESS = doc["mac_address"]; // "<MAC_ADDRESS>"
      const char* IP = doc["IP"];                   // "<IP_ADDRESS>"

      /* print json data */
      #ifdef DEBUG
      Serial.println("Mac address: " + String(MAC_ADDRESS));
      Serial.println("IP address: " + String(IP));
      Serial.println("Message: " + String(message));
      #endif

      // if (strcmp(MAC_ADDRESS, MAC_str) == 0){
      if ((String)MAC_ADDRESS != (String)MAC){
        #ifdef DEBUG
        Serial.println("Wrong mac address");
        #endif

        return 0;
      }
      else {
        #ifdef DEBUG
        Serial.println("Right mac address, return 1");
        #endif
        client.stop();

        return 1;
      }
    }
  } 

  else {
    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    Serial.println("Connection failed.");
    #endif

    return 0;
  }
}

/**
 * @brief Request gamestate from host
 * 
 * Request the gamestate from the host (raspberry pi). Receive the gamestate data in JSON format. 
 */
bool Gamestate(String server_path,String address){
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)) {

    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    String head = "--ESP32\r\nContent-Disposition: form-data; name=\"mac_address\"; filename=\"12345678\"\r\nContent-Type: mac_address\r\n\r\n";
    String tail = "\r\n--ESP32--\r\n";
    uint16_t totalLen  = head.length() + tail.length();

    #ifdef DEBUG
    Serial.println("Sending HTTP POST request...");
    #endif

    String SERVERPATH = server_path + address;

    /* Send HTTP request */ 
    client.println("GET " + SERVERPATH + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=ESP32");
    client.println();
    client.print(head);
    client.print(tail);

    Serial.println("Waiting for server response...");

    /* wait for response */ 
    while (!client.available()) {
      delay(100);
      Serial.print(".");
    }

    /* read response */
    String decoded_string;
    String input = client.readString();
    Serial.println(input);

    /* remove http overhead from input */
    int jsonStartIndex = input.indexOf("\r\n\r\n");

    if (jsonStartIndex != -1) {
      decoded_string = input.substring(jsonStartIndex + 4);
      decoded_string.trim();
  
      Serial.println("Extracted JSON content:");
      Serial.println(decoded_string);
    } 
    else {
      Serial.println("JSON content not found in HTTP response.");
      return 0;
    }

    /* parse JSON */
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, decoded_string);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    return 0;
    }   
    else {
      /* Access parsed JSON data */ 
      const char* Mac_address = doc["Mac_address"];
      const char* IP          = doc["IP"];
      const char* Team        = doc["Team"]; 
      const char* Team_color  = doc["Team_color"];
      int Team_score          = doc["Team_score"];
      int Enemy_team_score    = doc["Enemy_team_score"]; 
      int Hit_points          = doc["Hit_points"]; 
      int Hits                = doc["Hits"]; 

      /* print json data */
      #ifdef DEBUG
      Serial.println("Mac address: " + String(Mac_address));
      Serial.println("IP address: " + String(IP));
      Serial.println("Team: " + String(Team));
      Serial.println("Team color: " + String(Team_color));
      Serial.print("Team score: ");
      Serial.println(Team_score);
      Serial.print("Enemy team score: ");
      Serial.println(Enemy_team_score);
      Serial.print("Hit points: ");
      Serial.println(Hit_points);
      Serial.print("Hits: ");
      Serial.println(Hits);
      #endif
    }

    client.stop();
    return 1; 
  } 
  else {
    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    Serial.println("Connection failed.");
    #endif

    return 0;
  } 
}

/**
 * @brief Wait for a message
 * 
 * Wait to receive a message from the host. If a message is received, return 1. 
 */
int WaitForMessage(void){
  while (!client.available()){
    delay(100);
    Serial.print(".");
  }

  /* read response */
  String decoded_string;
  String input = client.readString();
  Serial.println(input);

  /* remove http overhead from input */
  int MessageIndex = input.indexOf("\r\n\r\n");

  if (MessageIndex != -1) {
    decoded_string = input.substring(MessageIndex + 4);
    decoded_string.trim();
  
    Serial.println("Extracted message content:");
    Serial.println(decoded_string);
  } 
  else {
    Serial.println("Error, no message content found.");
    return 0;
  }
  
  client.stop();

  if        (decoded_string == "Success") {
    return 1;
  } else if (decoded_string == "Error") {
    return 2;
  } else if (decoded_string.startsWith("Warning")) {
    return 3;
  } else {
    return 0;  // default case for other content
  }
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

    client.println("POST " + ServerPathShoot + " HTTP/1.1");
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

    client.stop();
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
