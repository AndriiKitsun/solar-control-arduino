#include "utils/wifi.h"

void connectToWiFi() {
  Serial.print(F("Connecting to "));
  Serial.print(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  while (WiFi.status() != WL_CONNECTED) {
    blinkLed();
    Serial.print(F("."));
    delay(500);
  }

  turnLedOff();

  Serial.println();
  Serial.print(F("Connected to "));
  Serial.println(WIFI_SSID);
  Serial.print(F("Device MAC: "));
  Serial.println(WiFi.macAddress());
}

JsonDocument getWiFiStatus() {
  JsonDocument doc;

  doc[F("ip")] = WiFi.localIP().toString();
  doc[F("mac")] = WiFi.macAddress();

  return doc;
}