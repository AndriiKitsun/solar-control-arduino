#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>
#include <GyverNTP.h>
#include <ESP8266HTTPClient.h>

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 inputAcPzem(pzemSWSerial, INPUT_AC_PZEM_ADDRESS);
PZEM004Tv30 outputAcPzem(pzemSWSerial, OUTPUT_AC_PZEM_ADDRESS);
ESP8266WebServer server(80);
GyverNTP ntp(TIMEZONE_OFFSET);

struct Date
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t ms;
};

struct Pzem
{
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
  float powerFactor;
  float t1Energy;
  float t2Energy;
  Date createdAt;
};

struct Zone
{
  float t1StartEnergy;
  float t2StartEnergy;
  float t1EnergyAcc;
  float t2EnergyAcc;
};

Zone inputAcPzemZone = {0.0, 0.0, 0.0, 0.0};
Zone outputAcPzemZone = {0.0, 0.0, 0.0, 0.0};

// Led
void initLedPins()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkLed()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

bool isWiFiConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

// Wifi

void connectToWiFi()
{
  Serial.print(F("Connecting to "));
  Serial.print(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  while (!isWiFiConnected())
  {
    blinkLed();
    Serial.print(F("."));
    delay(500);
  }

  Serial.println();
  Serial.print(F("Connected to WiFi. Device MAC: "));
  Serial.println(WiFi.macAddress());
}

// Time

uint8_t updateTime()
{
  return ntp.updateNow();
}

void startNTP()
{
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);

  if (status != 0)
  {
    Serial.print(F("NTP Status is not ok. Updating... "));

    uint8_t status = updateTime();

    Serial.print(F("Update status: "));
    Serial.println(status);
  }
}

void syncTime()
{
  ntp.tick();
}

Date getDate()
{
  return {
    year : ntp.year(),
    month : ntp.month(),
    day : ntp.day(),
    hour : ntp.hour(),
    minute : ntp.minute(),
    second : ntp.second(),
    ms : ntp.ms(),
  };
}

String toISODateString(const Date &date)
{
  String str;
  str.reserve(24);

  str += date.year;
  str += F("-");

  if (date.month < 10)
  {
    str += F("0");
  }
  str += date.month;
  str += F("-");

  if (date.day < 10)
  {
    str += F("0");
  }
  str += date.day;

  str += F("T");

  if (date.hour < 10)
  {
    str += F("0");
  }
  str += date.hour;
  str += F(":");

  if (date.minute < 10)
  {
    str += F("0");
  }
  str += date.minute;
  str += F(":");

  if (date.second < 10)
  {
    str += F("0");
  }
  str += date.second;
  str += F(".");

  if (date.ms < 100)
  {
    str += F("0");
  }
  str += date.ms;
  str += F("Z");

  return str;
}

String getNTPStatus()
{
  JsonDocument doc;
  String result;

  doc[F("status")] = ntp.status();
  doc[F("isSynced")] = ntp.synced();
  doc[F("isBusy")] = ntp.busy();
  doc[F("ping")] = ntp.ping();
  doc[F("isoDate")] = toISODateString(getDate());
  doc[F("tickStatus")] = ntp.tick();

  serializeJson(doc, result);

  return result;
}

String getNTPStatusWithUpdateStatus(uint8_t updateStatus)
{
  JsonDocument doc;
  String result;

  doc[F("status")] = ntp.status();
  doc[F("isSynced")] = ntp.synced();
  doc[F("isBusy")] = ntp.busy();
  doc[F("ping")] = ntp.ping();
  doc[F("isoDate")] = toISODateString(getDate());
  doc[F("tickStatus")] = ntp.tick();
  doc[F("updateStatus")] = updateStatus;

  serializeJson(doc, result);

  return result;
}

bool isT1ZoneActive(uint8_t hour)
{
  return hour >= 7 && hour < 23;
}

bool isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second)
{
  return hour == 7 && minute == 0 && second == 0;
}

bool isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second)
{
  return hour == 22 && minute == 59 && second == 59;
}

bool isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second)
{
  return hour == 23 && minute == 0 && second == 0;
}

bool isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second)
{
  return hour == 6 && minute == 59 && second == 59;
}

// Pzem

float calcT1ZoneEnergy(const Pzem &sensor, Zone &zone)
{
  bool isStartOfZone = isStartOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!zone.t1StartEnergy || isStartOfZone)
  {
    zone.t1StartEnergy = sensor.energy;
  }

  float t1Energy = sensor.energy - zone.t1StartEnergy + zone.t1EnergyAcc;

  if (isEndOfZone)
  {
    zone.t1EnergyAcc = t1Energy;
  }

  return t1Energy;
}

float calcT2ZoneEnergy(const Pzem &sensor, Zone &zone)
{
  bool isStartOfZone = isStartOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!zone.t2StartEnergy || isStartOfZone)
  {
    zone.t2StartEnergy = sensor.energy;
  }

  float t2Energy = sensor.energy - zone.t2StartEnergy + zone.t2EnergyAcc;

  if (isEndOfZone)
  {
    zone.t2EnergyAcc = t2Energy;
  }

  return t2Energy;
}

void calcZoneEnergy(Pzem &sensor, Zone &zone)
{
  float t1Energy;
  float t2Energy;

  if (isT1ZoneActive(sensor.createdAt.hour))
  {
    t1Energy = calcT1ZoneEnergy(sensor, zone);
    t2Energy = zone.t2EnergyAcc;
  }
  else
  {
    t1Energy = zone.t1EnergyAcc;
    t2Energy = calcT2ZoneEnergy(sensor, zone);
  }

  sensor.t1Energy = t1Energy;
  sensor.t2Energy = t2Energy;
}

Pzem getPzemValues(PZEM004Tv30 &pzem, Zone &zone)
{
  Pzem sensor;

  sensor.voltage = pzem.voltage();
  sensor.createdAt = getDate();

  if (isnan(sensor.voltage))
  {
    sensor.current = NAN;
    sensor.power = NAN;
    sensor.energy = NAN;
    sensor.frequency = NAN;
    sensor.powerFactor = NAN;
    sensor.t1Energy = NAN;
    sensor.t2Energy = NAN;
  }
  else
  {
    sensor.current = pzem.current();
    sensor.power = pzem.power() / 1000.0;
    sensor.energy = pzem.energy();
    sensor.frequency = pzem.frequency();
    sensor.powerFactor = pzem.pf();

    calcZoneEnergy(sensor, zone);
  }

  return sensor;
}

JsonDocument pzemToJson(const Pzem &sensor)
{
  JsonDocument doc;

  doc[F("voltageV")] = sensor.voltage;
  doc[F("currentA")] = sensor.current;
  doc[F("powerKw")] = sensor.power;
  doc[F("energyKwh")] = sensor.energy;
  doc[F("frequencyHz")] = sensor.frequency;
  doc[F("powerFactor")] = sensor.powerFactor;
  doc[F("t1EnergyKwh")] = sensor.t1Energy;
  doc[F("t2EnergyKwh")] = sensor.t2Energy;
  doc[F("createdAt")] = toISODateString(sensor.createdAt);

  return doc;
}

String changePzemAddress(uint8_t addr)
{
  JsonDocument doc;
  String result;

  doc[F("currentAddress")] = inputAcPzem.getAddress();
  doc[F("addressToSet")] = addr;
  doc[F("isConnected")] = !isnan(inputAcPzem.voltage());
  doc[F("isChanged")] = inputAcPzem.setAddress(addr);

  serializeJson(doc, result);

  return result;
}

void resetPzemCounter(PZEM004Tv30 &sensor, Zone &zone)
{
  sensor.resetEnergy();

  zone.t1StartEnergy = 0.0;
  zone.t2StartEnergy = 0.0;
  zone.t1EnergyAcc = 0.0;
  zone.t2EnergyAcc = 0.0;
}

void resetEnergyCounter()
{
  resetPzemCounter(inputAcPzem, inputAcPzemZone);
  resetPzemCounter(outputAcPzem, outputAcPzemZone);
}

// Server

// GET "/health"
void handleHealthCheck()
{
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("text/plain"), F("UP"));

  digitalWrite(LED_BUILTIN, HIGH);
}

String collectPzemPayload()
{
  JsonDocument doc;
  String payload;

  doc[F("inputAc")].to<JsonObject>();
  doc[F("outputAc")].to<JsonObject>();

  doc[F("inputAc")] = pzemToJson(getPzemValues(inputAcPzem, inputAcPzemZone));
  doc[F("outputAc")] = pzemToJson(getPzemValues(outputAcPzem, outputAcPzemZone));

  serializeJson(doc, payload);

  return payload;
}

// GET "/pzems"
void handlePzemValues()
{
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("application/json"), collectPzemPayload());

  digitalWrite(LED_BUILTIN, HIGH);
}

// PATCH "/pzems/address"
void handlePzemAddressChange()
{
  digitalWrite(LED_BUILTIN, LOW);

  long address = 0;

  for (uint8_t i = 0; i < server.args(); i++)
  {
    if (server.argName(i) == F("address"))
    {
      address = server.arg(i).toInt();
    }
  }

  if (address >= 1 && address <= 247)
  {
    server.send(HTTP_CODE_OK, F("application/json"), changePzemAddress(address));
  }
  else
  {
    server.send(HTTP_CODE_BAD_REQUEST, F("text/plain"), F("The \"address\" query param should be in the range between 1 and 247"));
  }

  digitalWrite(LED_BUILTIN, HIGH);
}

// DELETE "/pzems/counter"
void handleCounterReset()
{
  digitalWrite(LED_BUILTIN, LOW);

  resetEnergyCounter();

  server.send(HTTP_CODE_NO_CONTENT);

  digitalWrite(LED_BUILTIN, HIGH);
}

// GET "/time"
void handleTime()
{
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_OK, F("application/json"), getNTPStatus());

  digitalWrite(LED_BUILTIN, HIGH);
}

// POST "/time/sync"
void handleTimeUpdate()
{
  digitalWrite(LED_BUILTIN, LOW);

  uint8_t status = updateTime();

  server.send(HTTP_CODE_OK, F("application/json"), getNTPStatusWithUpdateStatus(status));

  digitalWrite(LED_BUILTIN, HIGH);
}

// "**"
void handleNotFound()
{
  digitalWrite(LED_BUILTIN, LOW);

  server.send(HTTP_CODE_NOT_FOUND, F("text/plain"), F("Route Not Found"));

  digitalWrite(LED_BUILTIN, HIGH);
}

void configRouter()
{
  server.on(F("/health"), HTTP_GET, handleHealthCheck);

  server.on(F("/pzems"), HTTP_GET, handlePzemValues);
  server.on(F("/pzems/address"), HTTP_PATCH, handlePzemAddressChange);
  server.on(F("/pzems/counter"), HTTP_DELETE, handleCounterReset);

  server.on(F("/time"), HTTP_GET, handleTime);
  server.on(F("/time/sync"), HTTP_POST, handleTimeUpdate);

  server.onNotFound(handleNotFound);
}

void startServer()
{
  if (MDNS.begin(ESP_DOMAIN_NAME))
  {
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

void handleClient()
{
  server.handleClient();
  MDNS.update();
}



// Client

void jsonPOST(String endpoint, String payload)
{
  WiFiClient client;
  HTTPClient http;

  Serial.print(F("[HTTP] POST "));
  Serial.println(endpoint);

  http.begin(client, endpoint);
  http.addHeader(F("Content-Type"), F("application/json"));

  // NB: HttpClient works synchronously
  int httpCode = http.POST(payload);

  if (httpCode > 0)
  {
    Serial.print(F("[HTTP] POST... status: "));
    Serial.println(httpCode);

    Serial.print(F("[HTTP] POST... response: "));
    Serial.println(http.getString());
  }
  else
  {
    Serial.print(F("[HTTP] POST... failed, error: "));
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}

void streamPzemValues()
{
  static unsigned long previousMillis;

  if (millis() - previousMillis >= WEBHOOK_INTERVAL)
  {
    previousMillis = millis();

    jsonPOST(WEBHOOK_ENDPOINT, collectPzemPayload());
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  connectToWiFi();
  startNTP();
  startServer();
}

void loop()
{
  if (!isWiFiConnected())
  {
    return;
  }

  syncTime();

  handleClient();
  streamPzemValues();
}
