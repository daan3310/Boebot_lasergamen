#include <Wireless_Communication/Wireless_Communication.h>

static NimBLEServer* pServer;
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
bool deviceConnected = false;

class ServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        Serial.println("Client connected");
        Serial.println("Multi-connect support: start advertising");
        NimBLEDevice::startAdvertising();
    };
    /** Alternative onConnect() method to extract details of the connection.
     *  See: src/ble_gap.h for the details of the ble_gap_conn_desc struct.
     */
    void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
        Serial.print("Client address: ");
        Serial.println(NimBLEAddress(desc->peer_ota_addr).toString().c_str());
        /** We can use the connection handle here to ask for different connection parameters.
         *  Args: connection handle, min connection interval, max connection interval
         *  latency, supervision timeout.
         *  Units; Min/Max Intervals: 1.25 millisecond increments.
         *  Latency: number of intervals allowed to skip.
         *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
         */
        pServer->updateConnParams(desc->conn_handle, 24, 48, 0, 60);
    };
    void onDisconnect(NimBLEServer* pServer) {
        Serial.println("Client disconnected - start advertising");
        NimBLEDevice::startAdvertising();
    };
    void onMTUChange(uint16_t MTU, ble_gap_conn_desc* desc) {
        Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, desc->conn_handle);
    };
};

 NimBLEService* pDeadService = pServer->createService("DEAD");
    NimBLECharacteristic* pBeefCharacteristic = pDeadService->createCharacteristic(
                                               "BEEF",
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE |
                               /** Require a secure connection for read and write access */
                                               NIMBLE_PROPERTY::READ_ENC |  // only allow reading if paired / encrypted
                                               NIMBLE_PROPERTY::WRITE_ENC   // only allow writing if paired / encrypted
                                              );
                                              

int SetupBLE()
{
  //Initialise bluetooth
  NimBLEDevice::init("Hello_World");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9); 
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  //Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pDeadService->getUUID());
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  return 0; 
}

int BLENotify()
{
  if(pServer->getConnectedCount()) {
        NimBLEService* pSvc = pServer->getServiceByUUID("BAAD");
        if(pSvc) {
            NimBLECharacteristic* pChr = pSvc->getCharacteristic("F00D");
            if(pChr) {
                pChr->notify(true);
            }
        }
    }
  return 0; 
}

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