#pragma once
#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "utils/led.h"

void connectToWiFi();

JsonDocument getWiFiStatus();

#endif
