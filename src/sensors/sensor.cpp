#include "sensors/sensor.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);

static AcPzem acInputPzem(AC_INPUT_SENSOR_NAME, acPzemSerial, 0, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutputPzem(AC_OUTPUT_SENSOR_NAME, acPzemSerial, 16, AC_OUTPUT_PZEM_ADDRESS);
static DcDivider dcDivider(DC_BATTERY_SENSOR_NAME);

void startSensors() {
  Serial.println(F("Initializing sensors"));

  acInputPzem.startPzem();
  acOutputPzem.startPzem();
  dcDivider.begin();
}

JsonDocument getSensorsValues() {
  JsonDocument doc;

  Date date = getLocalDate();

  doc[F("createdAtGmt")] = toJSON(getUTCDate());

  JsonArray pzems = doc[F("sensors")].to<JsonArray>();

  JsonDocument acInputValues = acInputPzem.getValues(date);
  JsonDocument acOutputValues = acOutputPzem.getValues(date);
  JsonDocument dcBatteryValues = dcDivider.getValues();

  if (!acInputValues.isNull()) {
    pzems.add(acInputValues);
  }

  if (!acOutputValues.isNull()) {
    pzems.add(acOutputValues);
  }

  if (!dcBatteryValues.isNull()) {
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