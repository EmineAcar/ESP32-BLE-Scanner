
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

int scanTime = 5; //In seconds
static BLEAddress *pMAC_Address;
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      if (advertisedDevice.haveName())
      {
        Serial.print("MAC Adress: ");
        pMAC_Address = new BLEAddress(advertisedDevice.getAddress());
        Serial.println(pMAC_Address->toString().c_str());
        Serial.print("RSSI: ");
        Serial.println(advertisedDevice.getRSSI());
        Serial.println("");
      }
     }
};
      
void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value

   Serial.println("Total heap: %d");
   Serial.println(ESP.getHeapSize());
   Serial.println("Free heap: %d");
   Serial.println(ESP.getFreeHeap());
   Serial.println("Total PSRAM: %d");
   Serial.println(ESP.getPsramSize());
   Serial.println("Free PSRAM: %d");
   Serial.println(ESP.getFreePsram());
}

void loop()
{
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delete pMAC_Address;
  pMAC_Address = NULL;
  delay(2000);
}
