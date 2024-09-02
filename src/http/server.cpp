#include "http/server.h"

static ESP8266WebServer server(80);

void startServer() {
  if (MDNS.begin(ESP_DOMAIN_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  configRouter();

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

// PATCH "/pzems/address?id={id}&address={1}"
void handlePzemAddressChange() {
  String id = server.arg(F("id"));
  long address = server.arg(F("address")).toInt();

  if (id.isEmpty()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"id\" param is required"));

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

  JsonDocument doc = changePzemAddress(id, address);

  if (doc.isNull()) {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered \"id\" is not found"));

    return;
  }

  String payload;

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// PATCH "/pzems/shunt?id={id}&shunt={1}"
void handlePzemShuntChange() {
  String id = server.arg(F("id"));
  long shuntType = server.arg(F("shunt")).toInt();

  if (id.isEmpty()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"id\" param is required"));

    return;
  }

  if (shuntType < 0 || shuntType > 3) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"shunt\" param should be in the range between 0 and 3"));

    return;
  }

  JsonDocument doc = changePzemShuntType(id, shuntType);

  if (doc.isNull()) {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered \"id\" is not found"));

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

// "**"
void handleNotFound() {
  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));
}
