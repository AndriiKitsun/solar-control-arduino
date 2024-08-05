#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "pzems/acpzem.h"
#include "utils/led.h"

void startServer();
void tickServer();

void configRouter();

void handleHealthCheck();
void handlePzemValues();
void handlePzemAddressChange();
void handlePzemsCounterReset();
void handleNotFound();

String getPzemsPayload();

#endif
