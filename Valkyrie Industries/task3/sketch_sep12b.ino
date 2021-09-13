
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// Define this for testing
#define __DEV__

//// Define the UUIDs for different devices

#ifndef ESP32_DEVICE_ID
#define ESP32_DEVICE_ID 1
#endif

#if ESP32_DEVICE_ID == 1
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RX_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TX_CHARACTERISTIC_UUID "d72deac6-d0a9-40aa-8068-ef6f32c24343"

//#define REMOTE_SERVICE_UUID "8d0e6813-ebdb-4996-ac4a-74b837129b71"
//#define REMOTE_RX_CHARACTERISTIC_UUID "b513e8a7-6109-4196-9c1b-530e08e162c0"
//#define REMOTE_TX_CHARACTERISTIC_UUID "9c821f7d-0e75-4ad4-a63d-c293daeaad19"

#define REMOTE_SERVICE_UUID "181C"
#define REMOTE_RX_CHARACTERISTIC_UUID "2A3D"
#define REMOTE_TX_CHARACTERISTIC_UUID "2A27"

#elif ESP32_DEVICE_ID == 2
#define REMOTE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define REMOTE_RX_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define REMOTE_TX_CHARACTERISTIC_UUID "d72deac6-d0a9-40aa-8068-ef6f32c24343"

#define SERVICE_UUID "8d0e6813-ebdb-4996-ac4a-74b837129b71"
#define RX_CHARACTERISTIC_UUID "b513e8a7-6109-4196-9c1b-530e08e162c0"
#define TX_CHARACTERISTIC_UUID "9c821f7d-0e75-4ad4-a63d-c293daeaad19"

#else
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RX_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TX_CHARACTERISTIC_UUID "d72deac6-d0a9-40aa-8068-ef6f32c24343"

#define REMOTE_SERVICE_UUID "8d0e6813-ebdb-4996-ac4a-74b837129b71"
#define REMOTE_RX_CHARACTERISTIC_UUID "b513e8a7-6109-4196-9c1b-530e08e162c0"
#define REMOTE_TX_CHARACTERISTIC_UUID "9c821f7d-0e75-4ad4-a63d-c293daeaad19"
#endif

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic = NULL;
#ifdef __DEV__
std::string rx_payload, tx_payload;
#else
uint8_t rx_payload[150], tx_payload[150];
#endif
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

static BLEUUID remoteServiceUUID(REMOTE_SERVICE_UUID);
static BLEUUID remoteRXCharUUID(REMOTE_RX_CHARACTERISTIC_UUID);
static BLEUUID remoteTXCharUUID(REMOTE_TX_CHARACTERISTIC_UUID);

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteRXCharacteristic;
static BLERemoteCharacteristic* pRemoteTXCharacteristic;
static BLEAdvertisedDevice* myDevice;

// Send Data Functions //

void sendData(BLECharacteristic *pCharacteristic, std::string message) {
  pCharacteristic->setValue(message);
  pCharacteristic->notify();
  delay(5);
}

void sendData(BLECharacteristic *pCharacteristic, uint8_t message[150]) {
  pCharacteristic->setValue(message, 150);
  pCharacteristic->notify();
  delay(5);
}

void sendData(BLERemoteCharacteristic* pRemoteCharacteristic, std::string message) {
  pRemoteCharacteristic->writeValue(message);
  delay(5);
}

void sendData(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t message[150]) {
  pRemoteCharacteristic->writeValue(message, 150);
  delay(5);
}

// Callbacks //

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      #ifdef __DEV__
      rx_payload.clear();
      #else
      memset(rx_payload, 0, 150);
      #endif
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("Received value: ");
        for (int i = 0; i < value.length() && i < 150; i++) {
          Serial.print(value[i]);
          rx_payload[i] = value[i];
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
//    Serial.print("BLE Advertised Device found: ");
//    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(remoteServiceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

// Connection to Server via the Client part //

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(remoteServiceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(remoteServiceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the TX characteristic in the service of the remote BLE server.
    pRemoteTXCharacteristic = pRemoteService->getCharacteristic(remoteTXCharUUID);
    if (pRemoteTXCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(remoteTXCharUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our TX characteristic");

    // Obtain a reference to the RX characteristic in the service of the remote BLE server.
    pRemoteRXCharacteristic = pRemoteService->getCharacteristic(remoteRXCharUUID);
    if (pRemoteRXCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(remoteRXCharUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our RX characteristic");

    // Read the value of the TX characteristic.
    if(!pRemoteTXCharacteristic->canRead()) {
//      std::string value = pRemoteTXCharacteristic->readValue();
//      Serial.print("The characteristic value was: ");
//      Serial.println(value.c_str());
      Serial.println("The TX Characteristic should have READ permission!");
    }

    if(pRemoteTXCharacteristic->canNotify())
      pRemoteTXCharacteristic->registerForNotify(notifyCallback);
    else Serial.println("The TX Characteristic should have NOTIFY permission!");

    connected = true;
    return true;
}

//#ifndef __DEV__
//// strlcpy implementation
//size_t strlcpy(char *dst, const char *src, size_t size)
//{
//    size_t len = 0;
//    while(size > 1 && *src)
//    {
//        *dst++ = *src++;
//        size--;
//        len++;
//    }
//    if(size > 0)
//        *dst = 0;
//    return len + strlen(src);    
//}
//#endif

// MAIN -  Setup and Loop functions

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP");
  
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
                                         RX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ   |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pTxCharacteristic = pService->createCharacteristic(
                                         TX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ   |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

  pTxCharacteristic->addDescriptor(new BLE2902());
  pRxCharacteristic->setCallbacks(new MyCallbacks());

//  pRxCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  pAdvertising->start();

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  String temp = Serial.readString();
  #ifdef __DEV__
  tx_payload = std::string(temp.c_str());
  #else
  memcpy(tx_payload, temp.c_str(), 150);
  #endif
  
  // Server loop
  if (deviceConnected) {
    if (temp != NULL)
      sendData(pTxCharacteristic, tx_payload);
  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
  // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
  }

  // Client Loop
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      doConnect = false;
    } else {
      Serial.println("We failed to connect to the server.");
    }
  }

  if (connected) {
    sendData(pRemoteRXCharacteristic, tx_payload);
  } else if (doScan) {
    doConnect = true;
  }
}
