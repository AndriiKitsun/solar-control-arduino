#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  initLedPins();
  connectToWiFi();
  startServer();
}

void loop() {
  // auto startTime = millis();
  // Serial.print(F("Start time: "));
  // Serial.print(startTime);
  // Serial.println(F(" ms"));

  // printPzemValues();

  // auto endTime = millis();
  // Serial.print(F("End time: "));
  // Serial.print(startTime);
  // Serial.println(F(" ms"));

  // Serial.print(F("Elapsed time: "));
  // Serial.print(endTime - startTime);
  // Serial.println(F(" ms"));

  // delay(10000);

  handleClient();
}
