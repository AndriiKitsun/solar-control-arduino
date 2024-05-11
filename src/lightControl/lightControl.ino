#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>
#include <GyverNTP.h>

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  connectToWiFi();
  startNTP();
  startServer();
}

void loop() {
  if (!isWiFiConnected()) {
    return;
  }

  syncTime();

  handleClient();
}
