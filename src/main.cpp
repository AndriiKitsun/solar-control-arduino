#include <Arduino.h>
#include "http/server.h"
#include "utils/date.h"
#include "utils/eeprom.h"
#include "utils/led.h"
#include "utils/wifi.h"
#include "websocket/websocket.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  connectToWiFi();

  startNTP();
  startEeprom();
  startServer();
  startWebSocket();
}

void loop() {
  tickNTP();
  tickServer();
  tickWebSocket();

  broadcastPzemz();
}
