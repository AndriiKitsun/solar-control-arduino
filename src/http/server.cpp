#include "http/server.h"

ESP8266WebServer server(80);

SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);
AcPzem acInputPzem(acPzemSerial, AC_INPUT_PZEM_ADDRESS);
AcPzem acOutputPzem(acPzemSerial, AC_OUTPUT_PZEM_ADDRESS);

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
  server.on(F("/health"), HTTP_GET, handleHealthCheck);

  server.on(F("/pzems"), HTTP_GET, handlePzemValues);
  server.on(F("/pzems/address"), HTTP_PATCH, handlePzemAddressChange);
  server.on(F("/pzems/counter"), HTTP_DELETE, handlePzemsCounterReset);

  server.onNotFound(handleNotFound);
}

// GET "/health"
void handleHealthCheck() {
  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));
}

// GET "/pzems"
void handlePzemValues() {
  server.send(HTTP_CODE_OK, F("application/json"), getPzemsPayload());
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

  JsonDocument doc;
  String payload;

  if (id == F("acInput")) {
    doc = acInputPzem.changeAddress(address);
  } else if (id == F("acOutput")) {
    doc = acOutputPzem.changeAddress(address);
  } else {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Pzem with input \"id\" is not found"));
    return;
  }

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// DELETE "/pzems/counter"
void handlePzemsCounterReset() {
  acInputPzem.resetCounter();
  acOutputPzem.resetCounter();

  server.send(HTTP_CODE_NO_CONTENT);
}

// "**"
void handleNotFound() {
  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));
}

// Other

String getPzemsPayload() {
  JsonDocument doc;
  String payload;

  Date date = getDate();

  doc[F("createdAtGmt")] = toISODateString(date);

  JsonDocument acInputValues = acInputPzem.getValues(date);
  JsonDocument acOutputValues = acOutputPzem.getValues(date);

  if (!acInputValues.isNull()) {
    doc[F("acInputPzem")] = acInputValues;
  }

  if (!acOutputValues.isNull()) {
    doc[F("acOutputPzem")] = acOutputValues;
  }

  serializeJson(doc, payload);

  return payload;
}
