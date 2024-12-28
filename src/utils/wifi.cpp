#include "utils/wifi.h"

static IPAddress ip(192, 168, 50, 190);     // Static IP Address
static IPAddress gateway(192, 168, 50, 1);  // Gateway IP Address (router)
static IPAddress subnet(255, 255, 255, 0);

void connectToWiFi() {
  Serial.print(F("Connecting to "));
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);

  bool isConfigured = WiFi.config(ip, gateway, subnet);

  Serial.print(F("ESP Static IP: "));
  Serial.println(isConfigured ? F("Configured") : F("Configuration Failed"));

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