#pragma once
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "http/server.h"

void startWebSocket();
void tickWebSocket();

void broadcastPzemz();

#endif
