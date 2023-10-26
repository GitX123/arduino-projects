#include <ArduinoBLE.h>
#include <WiFiNINA.h>
#include "uuid.h"

// BLE
BLEService wifiSetupService(WIFI_SETUP_SERVICE_UUID);
BLEStringCharacteristic ssidCharacteristic(SSID_CHARACTERISTIC_UUID, BLERead | BLEWrite, 32);
BLEStringCharacteristic passwordCharacteristic(PASSWORD_CHARACTERISTIC_UUID, BLERead | BLEWrite, 32);
BLEStringCharacteristic ipaddressCharacteristic(IPADDRESS_CHARACTERISTIC_UUID, BLERead | BLENotify, 16);

// Wifi
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Serial ready");
  Serial.println("------------------------------------");

  // BLE setup
  if (!BLE.begin()) {
    Serial.println("BLE init failed.");
    while (1);
  }

  wifiSetupService.addCharacteristic(ssidCharacteristic);
  wifiSetupService.addCharacteristic(passwordCharacteristic);
  wifiSetupService.addCharacteristic(ipaddressCharacteristic);
  BLE.addService(wifiSetupService);
  BLE.setDeviceName("Arduino");
  BLE.setAdvertisedService(wifiSetupService);
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    while (central.connected()) {
      if (status != WL_CONNECTED) {
        if (ssidCharacteristic.written() || passwordCharacteristic.written()) {
          String ssid = ssidCharacteristic.value();
          String password = passwordCharacteristic.value();
          // if ssid and password are not empty
          if (ssid.length() != 0 && password.length() != 0) {
            Serial.print("SSID: ");
            Serial.println(ssid);
            Serial.print("Password: ");
            Serial.println(password);
            if (WiFi.begin(ssid.c_str(), password.c_str()) != WL_CONNECTED) {
              Serial.println("WiFi connection failed");
            } else {
              Serial.println("WiFi connection succeeded");
            }
            Serial.println("------------------------------------");
          }
        }
      } 
    }
  }

  if (status == WL_CONNECTED) {
    Serial.print("IP address: ");
    Serial.println(ipToStr(WiFi.localIP()));
    Serial.println("------------------------------------");
    delay(1000);
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("[Connect] central: ");
  Serial.println(central.address());
  Serial.println("------------------------------------");
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("[Disconnect] central: ");
  Serial.println(central.address());
  Serial.println("------------------------------------");
}

String ipToStr(const IPAddress& ip) {
  return String(ip[0]) + String(".") +\
    String(ip[1]) + String(".") +\
    String(ip[2]) + String(".") +\
    String(ip[3]);
}