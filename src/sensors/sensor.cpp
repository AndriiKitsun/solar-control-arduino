#include "sensors/sensor.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);

static AcPzem acInputPzem(AC_INPUT_SENSOR_NAME, acPzemSerial, AC_INPUT_SENSOR_SAVE_ADDRESS, AC_INPUT_PZEM_ADDRESS, AC_SENSOR_AVG_VOLTAGE_SIZE);
static AcPzem acOutputPzem(AC_OUTPUT_SENSOR_NAME, acPzemSerial, AC_OUTPUT_SENSOR_SAVE_ADDRESS, AC_OUTPUT_PZEM_ADDRESS, AC_SENSOR_AVG_VOLTAGE_SIZE);
static DcDivider dcDivider(DC_BATTERY_SENSOR_NAME, DC_BATTERY_SENSOR_AVG_VOLTAGE_SIZE);

static bool protectionTriggered = false;

void startSensors() {
  Serial.println(F("Initializing sensors"));

  acInputPzem.startPzem();
  acOutputPzem.startPzem();
  dcDivider.start();
}

JsonDocument getSensorsValues() {
  JsonDocument doc;

  Date date = getLocalDate();

  doc[F("createdAt")] = toJSON(getUTCDate());
  doc[F("power")] = getPin(RELAY_PIN);

  JsonArray pzems = doc[F("sensors")].to<JsonArray>();

  JsonDocument acInputValues = acInputPzem.getValues(date);
  JsonDocument acOutputValues = acOutputPzem.getValues(date);
  JsonDocument dcBatteryValues = dcDivider.getValues();

  if (!acInputValues.isNull()) {
    pzems.add(acInputValues);
  }

  if (!acOutputValues.isNull()) {
    acOutputValues[F("protection")] = executeAcOutputProtection(acOutputValues);

    pzems.add(acOutputValues);
  }

  if (!dcBatteryValues.isNull()) {
    dcBatteryValues[F("protection")] = executeDcBatteryProtection(dcBatteryValues);

    pzems.add(dcBatteryValues);
  }

  doc[F("pTriggered")] = protectionTriggered;

  handleProtectionResult();

  return doc;
}

JsonDocument getPzemsStatus() {
  JsonDocument doc;

  doc.add(acInputPzem.getStatus());
  doc.add(acOutputPzem.getStatus());

  return doc;
}

JsonDocument resetPzemsCounter() {
  JsonDocument doc;

  doc.add(acInputPzem.resetCounter());
  doc.add(acOutputPzem.resetCounter());

  return doc;
}

JsonDocument changePzemAddress(String name, uint8_t address) {
  JsonDocument doc;

  if (name == F(AC_INPUT_SENSOR_NAME)) {
    doc = acInputPzem.changeAddress(address);
  } else if (name == F(AC_OUTPUT_SENSOR_NAME)) {
    doc = acOutputPzem.changeAddress(address);
  }

  return doc;
}

JsonDocument executeAcOutputProtection(const JsonDocument& data) {
  JsonDocument doc;

  bool isFrequency = checkProtection(AC_OUTPUT_FREQUENCY_RULE, data[F("frequency")]);
  bool isVoltage = checkProtection(AC_OUTPUT_VOLTAGE_RULE, data[F("voltage")]);
  bool isAvgVoltage = checkProtection(AC_OUTPUT_AVG_VOLTAGE_RULE, data[F("avgVoltage")]);

  bool result = isFrequency || isVoltage || isAvgVoltage;

  if (result) {
    protectionTriggered = true;
  }

  doc[AC_OUTPUT_FREQUENCY_RULE] = isFrequency;
  doc[AC_OUTPUT_VOLTAGE_RULE] = isVoltage;
  doc[AC_OUTPUT_AVG_VOLTAGE_RULE] = isAvgVoltage;

  return doc;
}

JsonDocument executeDcBatteryProtection(const JsonDocument& data) {
  JsonDocument doc;

  bool isAvgVoltage = checkProtection(DC_BATTERY_AVG_VOLTAGE_RULE, data[F("avgVoltage")]);

  if (isAvgVoltage) {
    protectionTriggered = true;
  }

  doc[DC_BATTERY_AVG_VOLTAGE_RULE] = isAvgVoltage;

  return doc;
}

void handleProtectionResult() {
  protectionTriggered ? pinLow(RELAY_PIN) : pinHigh(RELAY_PIN);

  protectionTriggered = false;
}