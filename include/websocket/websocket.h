#pragma once
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "pzems/pzem.h"

void startWebSocket();
void tickWebSocket();

void broadcastPzems();

#endif
