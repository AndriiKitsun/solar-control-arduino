#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <Arduino.h>
#include "http/server.h"

void jsonPOST(String endpoint, String payload);

void sendWebhook();

#endif
