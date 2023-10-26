#include <ArduinoBLE.h>

// BLE services and characteristics
BLEService ledService("180A"); // Device information
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite); // Digital output

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  // BLE initialization
  if (!BLE.begin()) {
    Serial.println("BLE init failed.");
    while (1);
  }

  // BLE advertising
  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  switchCharacteristic.writeValue(0); // init value
  BLE.advertise();
  Serial.println("BLE advertising started.");
}

void loop() {
  BLEDevice central = BLE.central(); // query central connected

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (switchCharacteristic.written()) {
        switch (switchCharacteristic.value()) {
          case 1:
            Serial.println("LED on");
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case 2:
            Serial.println("LED fast blink");
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            digitalWrite(LED_BUILTIN, LOW);
            break;
          case 3:
            Serial.println("LED slow blink");
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            break;
          default:
            Serial.println("LED off");
            digitalWrite(LED_BUILTIN, LOW);
            break;
        }
      }
    }

    // central disconnected
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);
  }
}
