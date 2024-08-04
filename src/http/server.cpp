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
  Serial.print(F("1. IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("2. URL: "));
  Serial.println(F("http://" ESP_DOMAIN_NAME ".local"));
}

void handleRequests() {
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
  blinkLed();

  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));

  blinkLed();
}

// GET "/pzems"
void handlePzemValues() {
  blinkLed();

  server.send(HTTP_CODE_OK, F("application/json"), getPzemsPayload());

  blinkLed();
}

// PATCH "/pzems/address?id={id}&address={1}"
void handlePzemAddressChange() {
  blinkLed();

  String id = server.arg(F("id"));
  long address = server.arg(F("address")).toInt();

  if (!id) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"id\" param is required"));
  } else if (!address) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" param is required"));
  } else if (address < 0x01 || address > 0xF7) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" param should be in the range between 1 and 247"));
  } else {
    JsonDocument doc;
    String payload;

    if (id == F("acInput")) {
      doc = acInputPzem.changeAddress(address);
    } else if (id == F("acOutput")) {
      doc = acOutputPzem.changeAddress(address);
    }

    serializeJson(doc, payload);

    server.send(HTTP_CODE_OK, F("application/json"), payload);
  }

  blinkLed();
}

// DELETE "/pzems/counter"
void handlePzemsCounterReset() {
  blinkLed();

  acInputPzem.resetCounter();
  acOutputPzem.resetCounter();

  server.send(HTTP_CODE_NO_CONTENT);

  blinkLed();
}

// "**"
void handleNotFound() {
  blinkLed();

  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));

  blinkLed();
}

// Other

String getPzemsPayload() {
  JsonDocument doc;
  String payload;

  doc[F("acInputPzem")].to<JsonObject>();
  doc[F("acOutputPzem")].to<JsonObject>();

  doc[F("acInputPzem")] = acInputPzem.getValues();
  doc[F("acOutputPzem")] = acOutputPzem.getValues();

  serializeJson(doc, payload);

  return payload;
}