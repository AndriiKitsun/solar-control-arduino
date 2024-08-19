#include <Arduino.h>
#include "http/server.h"
#include "utils/date.h"
#include "utils/led.h"
#include "utils/wifi.h"
#include "websocket/websocket.h"

void setup() {
  // Serial.begin(74880);
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  connectToWiFi();

  startNTP();
  startServer();
  startWebSocket();
}

void loop() {
  tickNTP();
  tickServer();
  tickWebSocket();

  broadcastPzemz();
}
