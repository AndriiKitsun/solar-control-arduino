#pragma once
#ifndef PROTECTION_RULE_H
#define PROTECTION_RULE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "utils/eeprom.h"

#define PROTECTION_RULES_CONFIG_ADDRESS 32

#define AC_OUTPUT_AVG_FREQUENCY_RULE F("acOutputAvgFrequency")
#define AC_OUTPUT_VOLTAGE_RULE F("acOutputVoltage")
#define AC_OUTPUT_AVG_VOLTAGE_RULE F("acOutputAvgVoltage")
#define DC_BATTERY_AVG_VOLTAGE_RULE F("dcBatteryAvgVoltage")

// sizeof: 8
struct ProtectionRule {
  float min;
  float max;
};

// sizeof: 32
struct ProtectionRulesConfig {
  ProtectionRule acOutputAvgFrequency;
  ProtectionRule acOutputVoltage;
  ProtectionRule acOutputAvgVoltage;
  ProtectionRule dcBatteryAvgVoltage;
};

enum ProtectionRuleSaveState {
  SAVE_STATE_SUCCESS,
  SAVE_STATE_ERROR,
  SAVE_STATE_NOT_FOUND,
  SAVE_STATE_NOT_MODIFIED,
};

void loadProtectionRules();
JsonDocument getProtectionRules();

ProtectionRuleSaveState saveProtectionRule(const JsonDocument& doc);
bool isEqual(const ProtectionRule& rule, const JsonDocument& doc);
ProtectionRule updateProtectionRule(const JsonDocument& doc);
ProtectionRuleSaveState storeProtectionRules();

bool checkProtection(String id, float value);

#endif
