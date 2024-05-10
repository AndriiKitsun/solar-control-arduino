#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

void initPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  initPins();
  connectToWiFi();
  startServer();
}

void loop() {
  handleClient();
}
