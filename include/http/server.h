#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "pzems/pzem.h"
#include "utils/wifi.h"

void startServer();
void tickServer();

void configRouter();

void handleEspHealthCheck();
void handleEspStatus();
void handlePzemValues();
void handlePzemAddressChange();
void handlePzemShuntChange();
void handlePzemsCounterReset();
void handleNotFound();

#endif
