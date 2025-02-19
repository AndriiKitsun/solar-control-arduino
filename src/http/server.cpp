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

  server.on(F("/sensors"), HTTP_GET, handleSensorsValues);

  server.on(F("/protection-rules"), HTTP_GET, handleProtectionRules);
  server.on(F("/protection-rules"), HTTP_PUT, handleProtectionRuleChange);

  server.on(F("/pzems/address"), HTTP_PATCH, handlePzemAddressChange);
  server.on(F("/pzems/counter"), HTTP_DELETE, handlePzemsCounterReset);

  server.on(F("/relays"), HTTP_GET, []() { handleRelayState(RELAY_GET); });
  server.on(F("/relays/on"), HTTP_POST, []() { handleRelayState(RELAY_ON); });
  server.on(F("/relays/off"), HTTP_POST, []() { handleRelayState(RELAY_OFF); });
  server.on(F("/relays/toggle"), HTTP_POST, []() { handleRelayState(RELAY_TOGGLE); });

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

// GET "/sensors"
void handleSensorsValues() {
  String payload;

  serializeJson(getSensorsValues(), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// GET "/protection-rules"
void handleProtectionRules() {
  String payload;

  serializeJson(getProtectionRules(), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// PUT "/protection-rules"
void handleProtectionRuleChange() {
  JsonDocument body;
  DeserializationError error = deserializeJson(body, server.arg("plain"));

  if (error) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The passed payload is not valid"));

    return;
  }

  if (body[F("id")].isNull()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"id\" field is required"));

    return;
  }

  if (body[F("min")].isNull()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"min\" field is required"));

    return;
  }

  if (body[F("max")].isNull()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"max\" field is required"));

    return;
  }

  ProtectionRuleSaveState saveResult = saveProtectionRule(body);

  if (saveResult == SAVE_STATE_NOT_FOUND) {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Rule with entered id is not found"));

    return;
  }

  if (saveResult == SAVE_STATE_ERROR) {
    server.send(HTTP_CODE_INTERNAL_SERVER_ERROR, F("text/plain"), F("Rule saving finished with error"));

    return;
  }

  server.send(HTTP_CODE_OK);
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

// DELETE "/pzems/counter"
void handlePzemsCounterReset() {
  String payload;

  serializeJson(resetPzemsCounter(), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// GET "/relays"
// POST "/relays/on"
// POST "/relays/off"
// POST "/relays/toggle"
void handleRelayState(RelayState state) {
  String payload;

  switch (state) {
    case RELAY_ON:
      pinHigh(RELAY_PIN);
      break;
    case RELAY_OFF:
      pinLow(RELAY_PIN);
      break;
    case RELAY_TOGGLE:
      togglePin(RELAY_PIN);
      break;

    default:
      break;
  }

  serializeJson(getPinStatus(RELAY_PIN), payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
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
