#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "classes/acPzem.h"
#include "classes/dcDivider.h"
#include "protection-rule.h"
#include "utils/pin.h"

#define AC_INPUT_SENSOR_NAME "acInput"
#define AC_OUTPUT_SENSOR_NAME "acOutput"
#define DC_BATTERY_SENSOR_NAME "dcBattery"

#define AC_INPUT_SENSOR_SAVE_ADDRESS 0
#define AC_OUTPUT_SENSOR_SAVE_ADDRESS 16

void startSensors();

JsonDocument getSensorsValues();

JsonDocument getPzemsStatus();
JsonDocument resetPzemsCounter();
JsonDocument changePzemAddress(String pzemId, uint8_t address);

JsonDocument executeAcOutputProtection(const JsonDocument& data);
JsonDocument executeDcBatteryProtection(const JsonDocument& data);

#endif
