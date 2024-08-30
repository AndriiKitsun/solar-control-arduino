#pragma once
#ifndef PZEM_H
#define PZEM_H

#include <Arduino.h>
#include "classes/acpzem.h"
#include "classes/dcpzem.h"

#define AC_INPUT_PZEM_ID "acInput"
#define AC_OUTPUT_PZEM_ID "acOutput"
#define DC_BATTERY_OUTPUT_PZEM_ID "dcBatteryOutput"

void startPzems();

JsonDocument getPzemsStatus();
JsonDocument getPzemsPayload();
JsonDocument resetPzemsCounter();
JsonDocument changePzemAddress(String pzemId, uint8_t address);
JsonDocument changePzemShuntType(String pzemId, uint8_t shuntType);

#endif
