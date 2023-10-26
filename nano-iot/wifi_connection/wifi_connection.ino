#include <WiFiNINA.h>
#include "wifi_secret.h"

char ssid[] = SSID;
char password[] = PASSWORD;
int status = WL_IDLE_STATUS;
unsigned long previousMillis = 0;
int ledState = LOW;
int intervalLED = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connecto to WiFi network: ");
    Serial.println(ssid);
    Serial.println("---------------------------------------");
    status = WiFi.begin(ssid, password);
    delay(10000);
  }

  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.println("---------------------------------------");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= intervalLED) {
    intervalLED = WiFi.RSSI() * -10;
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
}
