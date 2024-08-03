ESP8266WebServer server(80);

void startServer() {
  if (MDNS.begin(ESP_DOMAIN_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  configRouter();

  server.begin();

  Serial.println(F("HTTP server started on:"));
  Serial.print(F("1. IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("2. URL: "));
  Serial.println(F("http://" ESP_DOMAIN_NAME ".local"));
}

void handleClient() {
  server.handleClient();
  MDNS.update();
}

void configRouter() {
  server.on(F("/health"), HTTP_GET, handleHealthCheck);

  server.on(F("/pzems"), HTTP_GET, handlePzemValues);
  server.on(F("/pzems/address"), HTTP_PATCH, handlePzemAddressChange);
  server.on(F("/pzems/counter"), HTTP_DELETE, handleCounterReset);

  server.on(F("/time"), HTTP_GET, handleTime);
  server.on(F("/time/sync"), HTTP_POST, handleTimeUpdate);

  server.onNotFound(handleNotFound);
}

// GET "/health"
void handleHealthCheck() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));

  digitalWrite(LED_BUILTIN, HIGH);
}

// GET "/pzems"
void handlePzemValues() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("application/json"), collectPzemPayload());

  digitalWrite(LED_BUILTIN, HIGH);
}

// PATCH "/pzems/address"
void handlePzemAddressChange() {
  digitalWrite(LED_BUILTIN, LOW);

  long address = 0;

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == F("address")) {
      address = server.arg(i).toInt();
    }
  }

  if (address >= 1 && address <= 247) {
    // server.send(HTTP_CODE_OK, F("application/json"), changePzemAddress(address));
  } else {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" query param should be in the range between 1 and 247"));
  }

  digitalWrite(LED_BUILTIN, HIGH);
}

// DELETE "/pzems/counter"
void handleCounterReset() {
  digitalWrite(LED_BUILTIN, LOW);

  // resetEnergyCounter();

  server.send(HTTP_CODE_NO_CONTENT);

  digitalWrite(LED_BUILTIN, HIGH);
}

// GET "/time"
void handleTime() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("application/json"), getNTPStatus());

  digitalWrite(LED_BUILTIN, HIGH);
}

// POST "/time/sync"
void handleTimeUpdate() {
  digitalWrite(LED_BUILTIN, LOW);

  uint8_t status = updateTime();

  server.send(HTTP_CODE_OK, F("application/json"), getNTPStatusWithUpdateStatus(status));

  digitalWrite(LED_BUILTIN, HIGH);
}

// "**"
void handleNotFound() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));

  digitalWrite(LED_BUILTIN, HIGH);
}
