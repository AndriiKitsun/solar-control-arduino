#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "classes/acpzem.h"
#include "classes/dcDivider.h"

#define AC_INPUT_SENSOR_NAME "acInput"
#define AC_OUTPUT_SENSOR_NAME "acOutput"
#define DC_BATTERY_SENSOR_NAME "dcBattery"

void startSensors();

JsonDocument getSensorsValues();

JsonDocument getPzemsStatus();
JsonDocument resetPzemsCounter();
JsonDocument changePzemAddress(String pzemId, uint8_t address);

#endif
