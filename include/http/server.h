#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "pzems/pzem.h"
#include "utils/pin.h"
#include "utils/wifi.h"

enum RelayState {
  RELAY_GET,
  RELAY_ON,
  RELAY_OFF,
  RELAY_TOGGLE,
};

void startServer();
void tickServer();

void configRouter();

void handleEspHealthCheck();
void handleEspStatus();

void handleSensorsValues();

void handlePzemAddressChange();
void handlePzemsCounterReset();

void handleRelayState(RelayState state);

void handleNotFound();

#endif
