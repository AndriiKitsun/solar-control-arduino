#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
// #include <PZEM004Tv30.h>
#include <PZEM017v1.h>
#include <ArduinoJson.h>
#include <GyverNTP.h>
#include <ESP8266HTTPClient.h>

struct Date {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t ms;
};

struct Pzem {
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

struct DcPzem {
  float voltage;
  float current;
  float power;
  float energy;
  Date createdAt;

  uint8_t address;
  uint16_t shuntType;
  bool updated;
};


struct Zone {
  float t1StartEnergy;
  float t2StartEnergy;
  float t1EnergyAcc;
  float t2EnergyAcc;
};

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();

  setShunt();

  connectToWiFi();
  startNTP();
  startServer();
}

void loop() {
  if (!isWiFiConnected()) {
    return;
  }

  syncTime();

  handleClient();
  // streamPzemValues();
}
