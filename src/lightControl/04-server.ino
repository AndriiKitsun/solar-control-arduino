ESP8266WebServer server(80);

void startServer() {
  if (MDNS.begin(DOMAIN_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  configRouter();

  server.begin();

  Serial.println(F("HTTP server started on:"));
  Serial.print(F("1. IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("2. URL: "));
  Serial.println(F("http://" DOMAIN_NAME ".local"));
}

void handleClient() {
  server.handleClient();
  MDNS.update();
}

void configRouter() {
  server.on(F("/"), HTTP_GET, handleRoot);
  server.on(F("/health"), HTTP_GET, handleHealthCheck);
  server.on(F("/counter"), HTTP_DELETE, handleResetCounter);
  server.on(F("/time"), HTTP_GET, handleTime);
  server.on(F("/time/sync"), HTTP_POST, handleTimeUpdate);
  server.onNotFound(handleNotFound);
}

// GET "/"
void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("application/json"), collectPzemPayload());

  digitalWrite(LED_BUILTIN, HIGH);
}

// GET "/health"
void handleHealthCheck() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));

  digitalWrite(LED_BUILTIN, HIGH);
}

// DELETE "/counter"
void handleResetCounter() {
  digitalWrite(LED_BUILTIN, LOW);

  resetEnergyCounter();

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
