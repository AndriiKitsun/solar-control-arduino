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
  Serial.println("http://" + String(DOMAIN_NAME) + ".local");
}

void handleClient() {
  server.handleClient();
  MDNS.update();
}

void configRouter() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/health", HTTP_GET, handleHealthCheck);
  server.onNotFound(handleNotFound);
}

// GET "/"
void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(200, "text/plain", "Some data here");

  digitalWrite(LED_BUILTIN, HIGH);
}

// GET "/health"
void handleHealthCheck() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(200, "text/plain", "UP");

  digitalWrite(LED_BUILTIN, HIGH);
}

// "**"
void handleNotFound() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(404, "text/plain", "Route Not Found");

  digitalWrite(LED_BUILTIN, HIGH);
}
