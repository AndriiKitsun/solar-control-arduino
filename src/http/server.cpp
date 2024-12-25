#include "http/server.h"

static ESP8266WebServer server(80);

void startServer() {
  if (MDNS.begin(ESP_DOMAIN_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  configRouter();

  server.enableCORS(true);
  server.begin();

  Serial.println(F("HTTP server started on:"));
  Serial.print(F("1. IP address: http://"));
  Serial.println(WiFi.localIP());
  Serial.print(F("2. URL: "));
  Serial.println(F("http://" ESP_DOMAIN_NAME ".local"));
}

void tickServer() {
  server.handleClient();
  MDNS.update();
}

void configRouter() {
  server.on(F("/health"), HTTP_GET, handleEspHealthCheck);
  server.on(F("/status"), HTTP_GET, handleEspStatus);

  server.on(F("/pzems"), HTTP_GET, handlePzemValues);
  server.on(F("/pzems/address"), HTTP_PATCH, handlePzemAddressChange);
  server.on(F("/pzems/shunt"), HTTP_PATCH, handlePzemShuntChange);
  server.on(F("/pzems/counter"), HTTP_DELETE, handlePzemsCounterReset);

  server.on(F("/relay/on"), HTTP_POST, handleRelayTurnOn);
  server.on(F("/relay/off"), HTTP_POST, handleRelayTurnOff);
  server.on(F("/relay/toggle"), HTTP_POST, handleRelayToggle);

  server.onNotFound(handleNotFound);
}

// GET "/health"
void handleEspHealthCheck() {
  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));
}

// GET "/status"
void handleEspStatus() {
  JsonDocument doc;
  String payload;

  doc[F("wifi")] = getWiFiStatus();
  doc[F("date")] = getDateStatus();
  doc[F("pzems")] = getPzemsStatus();
  doc[F("eeprom")] = getEepromStatus();

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// GET "/pzems"
void handlePzemValues() {
  String payload;

  serializeJson(getPzemsPayload(), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// PATCH "/pzems/address?name={name}&address={1}"
void handlePzemAddressChange() {
  String name = server.arg(F("name"));
  long address = server.arg(F("address")).toInt();

  if (name.isEmpty()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"name\" param is required"));

    return;
  }

  if (!address) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" param is required"));

    return;
  }

  if (address < 0x01 || address > 0xF7) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" param should be in the range between 1 and 247"));

    return;
  }

  JsonDocument doc = changePzemAddress(name, address);

  if (doc.isNull()) {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered name is not found"));

    return;
  }

  String payload;

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// PATCH "/pzems/shunt?name={name}&shunt={1}"
void handlePzemShuntChange() {
  String name = server.arg(F("name"));
  long shuntType = server.arg(F("shunt")).toInt();

  if (name.isEmpty()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"name\" param is required"));

    return;
  }

  if (shuntType < 0 || shuntType > 3) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"shunt\" param should be in the range between 0 and 3"));

    return;
  }

  JsonDocument doc = changePzemShuntType(name, shuntType);

  if (doc.isNull()) {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered name is not found"));

    return;
  }

  String payload;

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// DELETE "/pzems/counter"
void handlePzemsCounterReset() {
  String payload;

  serializeJson(resetPzemsCounter(), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// POST "/relay/on"
void handleRelayTurnOn() {
  turnRelayOn(RELAY_PIN);

  server.send(HTTP_CODE_OK);
}

// POST "/relay/off"
void handleRelayTurnOff() {
  turnRelayOff(RELAY_PIN);

  server.send(HTTP_CODE_OK);
}

// POST "/relay/toggle"
void handleRelayToggle() {
  toggleRelay(RELAY_PIN);

  server.send(HTTP_CODE_OK);
}

// "**"
void handleNotFound() {
  if (server.method() == HTTP_OPTIONS) {
    server.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    server.send(HTTP_CODE_NO_CONTENT);

    return;
  }

  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));
}
