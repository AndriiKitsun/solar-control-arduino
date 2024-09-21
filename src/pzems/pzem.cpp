#include "pzems/pzem.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);
static SoftwareSerial dcBatteryPzemSerial(DC_BATTERY_PZEM_RX_PIN, DC_PZEM_SHARED_TX_PIN);
static SoftwareSerial dcSunPzemSerial(DC_SUN_PZEM_RX_PIN, DC_PZEM_SHARED_TX_PIN);

static AcPzem acInputPzem(AC_INPUT_PZEM_ID, acPzemSerial, 0, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutputPzem(AC_OUTPUT_PZEM_ID, acPzemSerial, 16, AC_OUTPUT_PZEM_ADDRESS);

static DcPzem dcBatteryPzem(DC_BATTERY_PZEM_ID, dcBatteryPzemSerial, 32, DC_BATTERY_PZEM_ADDRESS);
static DcPzem dcSunPzem(DC_SUN_PZEM_ID, dcSunPzemSerial, 48, DC_SUN_PZEM_ADDRESS);

void startPzems() {
  Serial.println(F("Initializing PZEM zones"));

  acInputPzem.startPzem();
  acOutputPzem.startPzem();
  dcBatteryPzem.startPzem();
  dcSunPzem.startPzem();
}

JsonDocument getPzemsPayload() {
  JsonDocument doc;

  Date date = getLocalDate();

  doc[F("createdAtGmt")] = toJSON(getUTCDate());

  JsonArray pzems = doc[F("pzems")].to<JsonArray>();

  JsonDocument acInputValues = acInputPzem.getValues(date);
  JsonDocument acOutputValues = acOutputPzem.getValues(date);
  JsonDocument dcBatteryValues = dcBatteryPzem.getValues(date);
  JsonDocument dcSunValues = dcSunPzem.getValues(date);

  if (!acInputValues.isNull()) {
    pzems.add(acInputValues);
  }

  if (!acOutputValues.isNull()) {
    pzems.add(acOutputValues);
  }

  if (!dcBatteryValues.isNull()) {
    pzems.add(dcBatteryValues);
  }

  if (!dcSunValues.isNull()) {
    pzems.add(dcSunValues);
  }

  return doc;
}

JsonDocument getPzemsStatus() {
  JsonDocument doc;

  doc.add(acInputPzem.getStatus());
  doc.add(acOutputPzem.getStatus());
  doc.add(dcBatteryPzem.getStatus());
  doc.add(dcSunPzem.getStatus());

  return doc;
}

JsonDocument resetPzemsCounter() {
  JsonDocument doc;

  doc.add(acInputPzem.resetCounter());
  doc.add(acOutputPzem.resetCounter());
  doc.add(dcBatteryPzem.resetCounter());
  doc.add(dcSunPzem.resetCounter());

  return doc;
}

JsonDocument changePzemAddress(String pzemId, uint8_t address) {
  JsonDocument doc;

  if (pzemId == F(AC_INPUT_PZEM_ID)) {
    doc = acInputPzem.changeAddress(address);
  } else if (pzemId == F(AC_OUTPUT_PZEM_ID)) {
    doc = acOutputPzem.changeAddress(address);
  } else if (pzemId == F(DC_BATTERY_PZEM_ID)) {
    doc = dcBatteryPzem.changeAddress(address);
  } else if (pzemId == F(DC_SUN_PZEM_ID)) {
    doc = dcSunPzem.changeAddress(address);
  }

  return doc;
}

JsonDocument changePzemShuntType(String pzemId, uint8_t shuntType) {
  JsonDocument doc;

  if (pzemId == F(DC_BATTERY_PZEM_ID)) {
    doc = dcBatteryPzem.changeShuntType(shuntType);
  } else if (pzemId == F(DC_SUN_PZEM_ID)) {
    doc = dcSunPzem.changeShuntType(shuntType);
  }

  return doc;
}