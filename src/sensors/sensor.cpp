#include "sensors/sensor.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);

static AcPzem acInputPzem(AC_INPUT_SENSOR_NAME, acPzemSerial, AC_INPUT_SENSOR_SAVE_ADDRESS, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutputPzem(AC_OUTPUT_SENSOR_NAME, acPzemSerial, AC_OUTPUT_SENSOR_SAVE_ADDRESS, AC_OUTPUT_PZEM_ADDRESS);
static DcDivider dcDivider(DC_BATTERY_SENSOR_NAME);

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

  if (isFrequency || isVoltage) {
    pinLow(RELAY_PIN);
  }

  doc[AC_OUTPUT_FREQUENCY_RULE] = isFrequency;
  doc[AC_OUTPUT_VOLTAGE_RULE] = isVoltage;

  return doc;
}

JsonDocument executeDcBatteryProtection(const JsonDocument& data) {
  JsonDocument doc;

  bool isVoltage = checkProtection(DC_BATTERY_VOLTAGE_RULE, data[F("voltage")]);

  if (isVoltage) {
    pinLow(RELAY_PIN);
  }

  doc[DC_BATTERY_VOLTAGE_RULE] = isVoltage;

  return doc;
}