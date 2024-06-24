/**
 * MAC addressen voor ESP-CAM:
 * A: 00:11:22:AA:BB:CC"
 * B: 01:11:22:AA:BB:CC"
 * C: 02:11:22:AA:BB:CC"
 * D: 03:11:22:AA:BB:CC"
 * 
 * controller mac adressen:
 * A controller    		 1912 = 70:EA:23:1B:FC:FF
 * B controller 	  	 3913 = 70:EA:24:1B:FC:FF
 * C controller 		   2811 = 70:EA:25:1B:FC:FF
 * D controller zonder code = 70:EA:26:1B:FC:FF 
 */

#include "IO-layer.h"

#include <WiFiUdp.h>

#define SERIAL_BAUD_RATE 115200  // Change baud rate as needed


extern int hitpoints;
extern int points;
byte RxB;

//char sendbuf[4] = {0};
//char receivebuf[4] = {0};

// Define callback flag and function
uint8_t my_post_trans_cb_flag = 0;

void my_post_trans_cb()
{
  my_post_trans_cb_flag = 1;
}

esp_err_t blocking_transmit_slave_serial(byte TxBuf)
{
  Serial.write(TxBuf);

  // Wait for incoming data
  //uint32_t start_time = millis();
  if (Serial.available()>0) {
    // if (millis() - start_time > 2000) {
    //   return ESP_ERR_TIMEOUT; // Return timeout error if not enough data received within timeout
    // }
    byte datarec = Serial.read();
    RxB = datarec;
  }

  // Read incoming data
  // int datarec = Serial.read() - '0';
  // RxB = datarec;
  //strcpy(RxBuf, stringrec.c_str());

  
  //return ESP_OK;
}

esp_err_t non_blocking_queue_transaction_slave_serial(byte TxBuf)
{
  // This function is blocking in the case of Serial communication
  return blocking_transmit_slave_serial(TxBuf);
}

#ifdef USE_WIFI 
const char* ssid           = "Leaphy Lasergame!";
const char* password       = "Leaphydebug1!";
String serverName          = "192.168.0.102";   
String ServerPathStartup   = "/startup";  // Flask upload route (voor image) (flask library python)
String ServerPathGamestate = "/gamestate/";
String ServerPathShoot     = "/shoot";
String ServerPathDebug     = "/ESP_DEBUG";
String MAC                 = MAC_ADDRESS_DEF;
const char* MAC_str        = MAC_ADDRESS_DEF;
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
  #ifdef DEBUG
  Serial.println("Connecting to HTTP server");
  #endif
  while(!connected){
    connected = connect_pi(ServerPathStartup, MAC);
    #ifdef DEBUG
    Serial.print(".");
    #endif
    delay(500);
  }
  #ifdef DEBUG
  Serial.println("Connected");
  #endif
}

/**
 * @brief Connect to the host
 * 
 * Het device stuurt een bericht naar de host met het mac-address om zich aan te melden.
 */
bool connect_pi(String server_path,String address) {
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)) {

    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    /* Http route */
    String head        = "--ESP32\r\nContent-Disposition: form-data; name=\"mac_address\"; filename=\"12345678\"\r\nContent-Type: mac_address\r\n\r\n";
    String tail        = "\r\n--ESP32--\r\n";
    String mac_address = address; 

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
      #ifdef DEBUG
      Serial.print(".");
      #endif
    }

    /* read response */
    String decoded_string;
    String input = client.readString();
    #ifdef DEBUG
    Serial.println(input);
    #endif

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

      /* vergelijk ontvangen mac address met eigen mac address */
      if ((String)MAC_ADDRESS != (String)MAC) {
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
 * @brief stuur debug message
 */
void SendMessage(String server_path, String debug_message){
  #ifdef DEBUG  
  Serial.println("Connecting to server: " + serverName);
  #endif 

  if (client.connect(serverName.c_str(), serverPort)) {

    #ifdef DEBUG
    Serial.println("Connection successful!");
    #endif

    /* Http route */
    String head        = "--ESP32\r\nContent-Disposition: form-data; name=\"message\"; filename=\"12345678\"\r\nContent-Type: message\r\n\r\n";
    String tail        = "\r\n--ESP32--\r\n";
    uint16_t totalLen  = head.length() + tail.length() + debug_message.length();

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
    client.print(debug_message);
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
    String input = client.readString();
    // Serial.println(input);
  } 

  else {
    #ifdef DEBUG
    Serial.print("Connection to ");
    Serial.print(serverName);
    Serial.println("Connection failed.");
    #endif
  }
}

/**
 * @brief Request gamestate from host
 * 
 * Request the gamestate from the host (raspberry pi). Receive the gamestate data in JSON format. 
 */
bool Gamestate(String server_path,String address){
  #ifdef DEBUG  
  Serial.println("REQUEST GAMESTATE");
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
    #ifdef DEBUG
    Serial.println("Waiting for server response...");
    #endif

    /* wait for response */ 
    while (!client.available()) {
      delay(100);
      #ifdef DEBUG
      Serial.print(".");
      #endif
    }

    /* read response */
    String decoded_string;
    String input = client.readString();
    #ifdef DEBUG
    Serial.println(input);
    #endif

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
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, decoded_string);

    if (error) {
      #ifdef DEBUG
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      #endif
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

      hitpoints = Hit_points;
      points = Hits;

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

WiFiUDP udp;
unsigned int localUdpPort = 5005;  // Local port to listen on
char incomingPacket[255];  // Buffer for incoming packets

/**
 * @brief Wait for a message
 * 
 * Wait to receive a message from the host. 
 */
int WaitForMessage(void){
  #ifdef DEBUG
  //Serial.print(".");
  #endif
  String Message;

  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';
      Message = incomingPacket;
    }
    #ifdef DEBUG
    Serial.printf("Received packet of size %d from %s:%d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    Serial.printf("Packet contents: %s\n", incomingPacket);
    #endif
  }

  delay(10);
  client.stop();

  if        (Message == "Game started") {
    return 1;
  } else if (Message == "Game over") {
    return 2;
  } else if (Message == "You've been hit") {
    return 3;
  } else if (Message == "You hit the target") {
    return 4;
  } else if (Message == "Gamestate has changed") {
    return 5;
  } else if (Message == "reset robot") {
    return 6;
  } else {
    return 0;
  }
}

IPAddress init_wifi(){
    #ifdef DEBUG
    Serial.println("going to init wifi");
    #endif
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout

    WiFi.mode(WIFI_STA);
    #ifdef DEBUG
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    #endif
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED){
      #ifdef DEBUG
      Serial.print(".");
      #endif
      delay(500);
    }
    #ifdef DEBUG
    Serial.println();
    Serial.print("ESP32-CAM IP Address: ");
    Serial.println(WiFi.localIP());
    #endif

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); // enable brownout

    udp.begin(localUdpPort);
    #ifdef DEBUG
    Serial.printf("Now listening on UDP port %d\n", localUdpPort);

    Serial.println("done init wifi");
    #endif
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
    #ifdef DEBUG
    Serial.println("Camera capture failed");
    #endif
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
    #ifdef DEBUG
    Serial.println("Waiting for server response...");
    #endif

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
    #ifdef DEBUG
    Serial.printf("Camera init failed with error 0x%x", err);
    #endif
    }
    return err;
}

#endif
