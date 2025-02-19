#include "sensors/protection-rule.h"

static ProtectionRulesConfig config;

void loadProtectionRules() {
  getValue(PROTECTION_RULES_CONFIG_ADDRESS, config);
}

JsonDocument getProtectionRules() {
  JsonDocument doc;

  JsonObject rule1 = doc.add<JsonObject>();
  rule1["id"] = AC_OUTPUT_FREQUENCY_RULE;
  rule1["min"] = config.acOutputFrequency.min;
  rule1["max"] = config.acOutputFrequency.max;

  JsonObject rule2 = doc.add<JsonObject>();
  rule2["id"] = AC_OUTPUT_VOLTAGE_RULE;
  rule2["min"] = config.acOutputVoltage.min;
  rule2["max"] = config.acOutputVoltage.max;

  JsonObject rule3 = doc.add<JsonObject>();
  rule3["id"] = DC_BATTERY_VOLTAGE_RULE;
  rule3["min"] = config.dcBatteryVoltage.min;
  rule3["max"] = config.dcBatteryVoltage.max;

  return doc;
}

ProtectionRuleSaveState saveProtectionRule(const JsonDocument& doc) {
  String id = doc[F("id")];

  if (id == AC_OUTPUT_FREQUENCY_RULE) {
    config.acOutputFrequency = updateProtectionRule(doc);

    return storeProtectionRules();
  } else if (id == AC_OUTPUT_VOLTAGE_RULE) {
    config.acOutputVoltage = updateProtectionRule(doc);

    return storeProtectionRules();
  } else if (id == DC_BATTERY_VOLTAGE_RULE) {
    config.dcBatteryVoltage = updateProtectionRule(doc);

    return storeProtectionRules();
  }

  return SAVE_STATE_NOT_FOUND;
}

ProtectionRule updateProtectionRule(const JsonDocument& doc) {
  ProtectionRule rule;

  rule.min = doc[F("min")];
  rule.max = doc[F("max")];

  return rule;
}

ProtectionRuleSaveState storeProtectionRules() {
  bool isSuccess = putValue(PROTECTION_RULES_CONFIG_ADDRESS, config);

  if (isSuccess) {
    return SAVE_STATE_SUCCESS;
  }

  return SAVE_STATE_ERROR;
}

bool checkProtection(String id, float value) {
  ProtectionRule rule;

  if (id == AC_OUTPUT_FREQUENCY_RULE) {
    rule = config.acOutputFrequency;
  } else if (id == AC_OUTPUT_VOLTAGE_RULE) {
    rule = config.acOutputVoltage;
  } else if (id == DC_BATTERY_VOLTAGE_RULE) {
    rule = config.dcBatteryVoltage;
  } else {
    return false;
  }

  if (!value || !rule.min || !rule.max) {
    return false;
  }

  return value < rule.min || value > rule.max;
}