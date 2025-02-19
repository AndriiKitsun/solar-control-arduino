#pragma once
#ifndef PROTECTION_RULE_H
#define PROTECTION_RULE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "utils/eeprom.h"

#define PROTECTION_RULES_CONFIG_ADDRESS 32

#define AC_OUTPUT_FREQUENCY_RULE F("acOutputFrequency")
#define AC_OUTPUT_VOLTAGE_RULE F("acOutputVoltage")
#define DC_BATTERY_VOLTAGE_RULE F("dcBatteryVoltage")

// sizeof: 8
struct ProtectionRule {
  float min;
  float max;
};

// sizeof: 24
struct ProtectionRulesConfig {
  ProtectionRule acOutputFrequency;
  ProtectionRule acOutputVoltage;
  ProtectionRule dcBatteryVoltage;
};

enum ProtectionRuleSaveState {
  SAVE_STATE_SUCCESS,
  SAVE_STATE_ERROR,
  SAVE_STATE_NOT_FOUND,
};

void loadProtectionRules();
JsonDocument getProtectionRules();

ProtectionRuleSaveState saveProtectionRule(const JsonDocument& data);
ProtectionRule updateProtectionRule(const JsonDocument& doc);
ProtectionRuleSaveState storeProtectionRules();

bool checkProtection(String id, float value);

#endif
