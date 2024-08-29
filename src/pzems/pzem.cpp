#include "pzems/pzem.h"

static SoftwareSerial acPzemSerial(AC_PZEM_RX_PIN, AC_PZEM_TX_PIN);
static SoftwareSerial dcPzemSerial(DC_PZEM_RX_PIN, DC_PZEM_TX_PIN);

static AcPzem acInPzem(acPzemSerial, 0, AC_INPUT_PZEM_ADDRESS);
static AcPzem acOutPzem(acPzemSerial, 16, AC_OUTPUT_PZEM_ADDRESS);

static DcPzem dcBattOutPzem(dcPzemSerial, 32, DC_BATTERY_OUTPUT_PZEM_ADDRESS);

void startPzems() {
  Serial.println(F("Initializing PZEM zones"));

  acInPzem.startPzem();
  acOutPzem.startPzem();
  dcBattOutPzem.startPzem();
}

JsonDocument getPzemsPayload() {
  JsonDocument doc;

  Date date = getLocalDate();

  doc[F("createdAtGmt")] = toJSON(getUTCDate());

  doc[F(AC_INPUT_PZEM_ID)] = acInPzem.getValues(date);
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutPzem.getValues(date);
  doc[F(DC_BATTERY_OUTPUT_PZEM_ID)] = dcBattOutPzem.getValues(date);

  return doc;
}

JsonDocument getPzemsStatus() {
  JsonDocument doc;

  doc[F(AC_INPUT_PZEM_ID)] = acInPzem.getStatus();
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutPzem.getStatus();
  doc[F(DC_BATTERY_OUTPUT_PZEM_ID)] = dcBattOutPzem.getStatus();

  return doc;
}

JsonDocument resetPzemsCounter() {
  JsonDocument doc;

  doc[F(AC_INPUT_PZEM_ID)] = acInPzem.resetCounter();
  doc[F(AC_OUTPUT_PZEM_ID)] = acOutPzem.resetCounter();
  doc[F(DC_BATTERY_OUTPUT_PZEM_ID)] = dcBattOutPzem.resetCounter();

  return doc;
}

JsonDocument changePzemAddress(String pzemId, uint8_t address) {
  JsonDocument doc;

  if (pzemId == F(AC_INPUT_PZEM_ID)) {
    doc = acInPzem.changeAddress(address);
  } else if (pzemId == F(AC_OUTPUT_PZEM_ID)) {
    doc = acOutPzem.changeAddress(address);
  } else if (pzemId == F(DC_BATTERY_OUTPUT_PZEM_ID)) {
    doc = dcBattOutPzem.changeAddress(address);
  }

  return doc;
}

JsonDocument changePzemShuntType(String pzemId, uint8_t shuntType) {
  JsonDocument doc;

  if (pzemId == F(DC_BATTERY_OUTPUT_PZEM_ID)) {
    doc = dcBattOutPzem.changeShuntType(shuntType);
  }

  return doc;
}