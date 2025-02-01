#include <Arduino.h>
#include "http/server.h"
#include "sensors/sensor.h"
#include "utils/date.h"
#include "utils/eeprom.h"
#include "utils/led.h"
#include "utils/pin.h"
#include "utils/wifi.h"
#include "websocket/websocket.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();
  initPin(RELAY_PIN, HIGH);

  connectToWiFi();

  startNTP();
  startEeprom();
  startSensors();
  startServer();
  startWebSocket();
}

void loop() {
  tickNTP();
  tickServer();
  tickWebSocket();

  broadcastPzems();
}
