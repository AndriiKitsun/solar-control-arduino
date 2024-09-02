#include "pzems/pzem.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);
static SoftwareSerial dcBatteryPzemSerial(DC_BATTERY_PZEM_RX_PIN, DC_PZEM_SHARED_TX_PIN);
static SoftwareSerial dcSunPzemSerial(DC_SUN_PZEM_RX_PIN, DC_PZEM_SHARED_TX_PIN);

static AcPzem acInputPzem(acPzemSerial, 0, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutputPzem(acPzemSerial, 16, AC_OUTPUT_PZEM_ADDRESS);

static DcPzem dcBatteryPzem(dcBatteryPzemSerial, 32, DC_BATTERY_PZEM_ADDRESS);
static DcPzem dcSunPzem(dcSunPzemSerial, 48, DC_SUN_PZEM_ADDRESS);

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

  doc[F(AC_INPUT_PZEM_ID)] = acInputPzem.getValues(date);
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutputPzem.getValues(date);
  doc[F(DC_BATTERY_PZEM_ID)] = dcBatteryPzem.getValues(date);
  doc[F(DC_SUN_PZEM_ID)] = dcSunPzem.getValues(date);

  return doc;
}

JsonDocument getPzemsStatus() {
  JsonDocument doc;

  doc[F(AC_INPUT_PZEM_ID)] = acInputPzem.getStatus();
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutputPzem.getStatus();
  doc[F(DC_BATTERY_PZEM_ID)] = dcBatteryPzem.getStatus();
  doc[F(DC_SUN_PZEM_ID)] = dcSunPzem.getStatus();

  return doc;
}

JsonDocument resetPzemsCounter() {
  JsonDocument doc;

  doc[F(AC_INPUT_PZEM_ID)] = acInputPzem.resetCounter();
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutputPzem.resetCounter();
  doc[F(DC_BATTERY_PZEM_ID)] = dcBatteryPzem.resetCounter();
  doc[F(DC_SUN_PZEM_ID)] = dcSunPzem.resetCounter();

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