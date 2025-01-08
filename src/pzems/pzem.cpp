#include "pzems/pzem.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);

static AcPzem acInputPzem(AC_INPUT_PZEM_NAME, acPzemSerial, 0, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutputPzem(AC_OUTPUT_PZEM_NAME, acPzemSerial, 16, AC_OUTPUT_PZEM_ADDRESS);

static DcPzem dcBatteryPzem(DC_BATTERY_PZEM_NAME, DC_BATTERY_PZEM_RO_PIN, DC_BATTERY_PZEM_RE_DE_PIN, DC_BATTERY_PZEM_DI_PIN, 32, DC_BATTERY_PZEM_ADDRESS);

void startPzems() {
  Serial.println(F("Initializing PZEMs"));

  // Workaround to pass void callbacks in class
  dcBatteryPzem._preTransmissionCb = []() { dcBatteryPzem.preTransmission(); };
  dcBatteryPzem._postTransmissionCb = []() { dcBatteryPzem.postTransmission(); };

  acInputPzem.startPzem();
  acOutputPzem.startPzem();
  dcBatteryPzem.startPzem();
}

JsonDocument getPzemsPayload() {
  JsonDocument doc;

  Date date = getLocalDate();

  doc[F("createdAtGmt")] = toJSON(getUTCDate());

  JsonArray pzems = doc[F("pzems")].to<JsonArray>();

  JsonDocument acInputValues = acInputPzem.getValues(date);
  JsonDocument acOutputValues = acOutputPzem.getValues(date);
  JsonDocument dcBatteryValues = dcBatteryPzem.getValues(date);

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
  doc.add(dcBatteryPzem.getStatus());

  return doc;
}

JsonDocument resetPzemsCounter() {
  JsonDocument doc;

  doc.add(acInputPzem.resetCounter());
  doc.add(acOutputPzem.resetCounter());
  doc.add(dcBatteryPzem.resetCounter());

  return doc;
}

JsonDocument changePzemAddress(String name, uint8_t address) {
  JsonDocument doc;

  if (name == F(AC_INPUT_PZEM_NAME)) {
    doc = acInputPzem.changeAddress(address);
  } else if (name == F(AC_OUTPUT_PZEM_NAME)) {
    doc = acOutputPzem.changeAddress(address);
  } else if (name == F(DC_BATTERY_PZEM_NAME)) {
    doc = dcBatteryPzem.changeAddress(address);
  }

  return doc;
}

JsonDocument changePzemShuntType(String name, uint8_t shuntType) {
  JsonDocument doc;

  if (name == F(DC_BATTERY_PZEM_NAME)) {
    doc = dcBatteryPzem.changeShuntType(shuntType);
  }

  return doc;
}