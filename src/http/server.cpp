#include "http/server.h"

ESP8266WebServer server(80);

SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);
SoftwareSerial dcPzemSerial(DC_PZEM_RX_PIN, DC_PZEM_TX_PIN);

AcPzem acInPzem(acPzemSerial, AC_INPUT_PZEM_ADDRESS);
AcPzem acOutPzem(acPzemSerial, AC_OUTPUT_PZEM_ADDRESS);

DcPzem dcBattOutPzem(dcPzemSerial, DC_BATTERY_OUTPUT_PZEM_ADDRESS);

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
  server.send(HTTP_CODE_OK, F("application/json"), getEspStatus());
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
    doc = acInPzem.changeAddress(address);
  } else if (id == F("acOutput")) {
    doc = acOutPzem.changeAddress(address);
  } else if (id == F("dcBatteryOutput")) {
    doc = dcBattOutPzem.changeAddress(address);
  } else {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered \"id\" is not found"));
    return;
  }

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// PATCH "/pzems/shunt?id={id}&shunt={1}"
void handlePzemShuntChange() {
  String id = server.arg(F("id"));
  long shuntType = server.arg(F("shunt")).toInt();

  Serial.print("Enteret Shunt: ");
  Serial.println(shuntType);

  if (id.isEmpty()) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"id\" param is required"));
    return;
  }

  if (shuntType < 0 || shuntType > 3) {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"shunt\" param should be in the range between 0 and 3"));
    return;
  }

  JsonDocument doc;
  String payload;

  if (id == F("dcBatteryOutput")) {
    doc = dcBattOutPzem.changeShuntType(shuntType);
  } else {
    server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("PZEM with entered \"id\" is not found"));
    return;
  }

  serializeJson(doc, payload);

  server.send(HTTP_CODE_OK, F("application/json"), payload);
}

// DELETE "/pzems/counter"
void handlePzemsCounterReset() {
  acInPzem.resetCounter();
  acOutPzem.resetCounter();
  dcBattOutPzem.resetCounter();

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

  Date date = getLocalDate();

  doc[F("createdAtGmt")] = toJSON(getUTCDate());

  doc[F("acInput")] = acInPzem.getValues(date);
  doc[F("acOutput")] = acOutPzem.getValues(date);
  doc[F("dcBatteryOutput")] = dcBattOutPzem.getValues(date);

  serializeJson(doc, payload);

  return payload;
}

String getEspStatus() {
  JsonDocument doc;
  String payload;

  JsonObject date = doc[F("date")].to<JsonObject>();
  date[F("createdAtGmt")] = toJSON(getUTCDate());
  date[F("createdAt")] = toJSON(getLocalDate());

  JsonObject pzems = doc[F("pzems")].to<JsonObject>();
  pzems[F("acInputPzem")] = acInPzem.getStatus();
  pzems[F("acOutputPzem")] = acOutPzem.getStatus();
  pzems[F("dcBatteryOutputPzem")] = dcBattOutPzem.getStatus();

  doc["eeprom"]["isConnected"] = isEepromConnected();

  serializeJson(doc, payload);

  return payload;
}
