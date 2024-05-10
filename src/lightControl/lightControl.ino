#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "config.h"

ESP8266WebServer server(80);

void connectToWiFi() {
  Serial.print(F("Connecting to "));
  Serial.print(STASSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("WiFi connected"));
}

void startServer() {
  if (MDNS.begin(DOMAIN_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  // Routing
  server.on("/", handleRoot);
  server.on("/health", handleHealthCheck);
  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println(F("HTTP server started on:"));
  Serial.print(F("1. IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("2. URL: "));
  Serial.println("http://" + String(DOMAIN_NAME) + ".local");
}

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(200, "text/plain", "Some data here");

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleHealthCheck() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(200, "text/plain", "UP");

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleNotFound() {
  digitalWrite(LED_BUILTIN, LOW);

  delay(2000);
  server.send(404, "text/plain", "Route Not Found");

  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();

  connectToWiFi();
  startServer();
}

void loop() {
  server.handleClient();
  MDNS.update();
}
