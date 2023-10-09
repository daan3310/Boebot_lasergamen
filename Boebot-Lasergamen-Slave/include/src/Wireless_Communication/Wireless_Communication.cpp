#include <Wireless_Communication/Wireless_Communication.h>



int SetupWifi()
{
  const char* ssid = SSID__;
  const char* password = PASSWORD__;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  return 0; 
}