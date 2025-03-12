#include "sensors/protection-rule.h"

static ProtectionRulesConfig config;

void loadProtectionRules() {
  getValue(PROTECTION_RULES_CONFIG_ADDRESS, config);
}

JsonDocument getProtectionRules() {
  JsonDocument doc;

  JsonObject acOutputFrequency = doc.add<JsonObject>();
  acOutputFrequency["id"] = AC_OUTPUT_FREQUENCY_RULE;
  acOutputFrequency["min"] = config.acOutputFrequency.min;
  acOutputFrequency["max"] = config.acOutputFrequency.max;

  JsonObject acOutputVoltage = doc.add<JsonObject>();
  acOutputVoltage["id"] = AC_OUTPUT_VOLTAGE_RULE;
  acOutputVoltage["min"] = config.acOutputVoltage.min;
  acOutputVoltage["max"] = config.acOutputVoltage.max;

  JsonObject acOutputAvgVoltage = doc.add<JsonObject>();
  acOutputAvgVoltage["id"] = AC_OUTPUT_AVG_VOLTAGE_RULE;
  acOutputAvgVoltage["min"] = config.acOutputAvgVoltage.min;
  acOutputAvgVoltage["max"] = config.acOutputAvgVoltage.max;

  JsonObject dcBatteryAvgVoltage = doc.add<JsonObject>();
  dcBatteryAvgVoltage["id"] = DC_BATTERY_AVG_VOLTAGE_RULE;
  dcBatteryAvgVoltage["min"] = config.dcBatteryAvgVoltage.min;
  dcBatteryAvgVoltage["max"] = config.dcBatteryAvgVoltage.max;

  return doc;
}

ProtectionRuleSaveState saveProtectionRule(const JsonDocument& doc) {
  String id = doc[F("id")];

  auto processRule = [&](ProtectionRule& rule) -> ProtectionRuleSaveState {
    if (isEqual(rule, doc)) {
      return SAVE_STATE_NOT_MODIFIED;
    }

    rule = updateProtectionRule(doc);

    return storeProtectionRules();
  };

  if (id == AC_OUTPUT_FREQUENCY_RULE) {
    return processRule(config.acOutputFrequency);
  } else if (id == AC_OUTPUT_VOLTAGE_RULE) {
    return processRule(config.acOutputVoltage);
  } else if (id == AC_OUTPUT_AVG_VOLTAGE_RULE) {
    return processRule(config.acOutputAvgVoltage);
  } else if (id == DC_BATTERY_AVG_VOLTAGE_RULE) {
    return processRule(config.dcBatteryAvgVoltage);
  }

  return SAVE_STATE_NOT_FOUND;
}

bool isEqual(const ProtectionRule& rule, const JsonDocument& doc) {
  return rule.min == (float)doc[F("min")] && rule.max == (float)doc[F("max")];
}

ProtectionRule updateProtectionRule(const JsonDocument& doc) {
  ProtectionRule rule;

  rule.min = doc[F("min")];
  rule.max = doc[F("max")];

  return rule;
}

ProtectionRuleSaveState storeProtectionRules() {
  bool isSuccess = putValue(PROTECTION_RULES_CONFIG_ADDRESS, config);

  if (!isSuccess) {
    return SAVE_STATE_ERROR;
  }

  return SAVE_STATE_SUCCESS;
}

bool checkProtection(String id, float value) {
  ProtectionRule rule;

  if (id == AC_OUTPUT_FREQUENCY_RULE) {
    rule = config.acOutputFrequency;
  } else if (id == AC_OUTPUT_VOLTAGE_RULE) {
    rule = config.acOutputVoltage;
  } else if (id == AC_OUTPUT_AVG_VOLTAGE_RULE) {
    rule = config.acOutputAvgVoltage;
  } else if (id == DC_BATTERY_AVG_VOLTAGE_RULE) {
    rule = config.dcBatteryAvgVoltage;
  } else {
    return false;
  }

  if (!value || !rule.min || !rule.max) {
    return false;
  }

  return value < rule.min || value > rule.max;
}