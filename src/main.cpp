#include <Arduino.h>
#include "http/client.h"
#include "http/server.h"
#include "utils/date.h"
#include "utils/led.h"
#include "utils/wifi.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  connectToWiFi();
  startNTP();
  startServer();
}

void loop() {
  tickNTP();

  handleRequests();
  sendWebhook();
}
