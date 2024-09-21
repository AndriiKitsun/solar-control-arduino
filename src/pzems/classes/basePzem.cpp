#include "pzems/classes/basePzem.h"

// Public

BasePzem::BasePzem(String id, uint8_t storageAddress)
    : _id(id), _storageAddress(storageAddress) {}

void BasePzem::startPzem() {
  _zone = getZone();

  Serial.print(F("Zone data on: "));
  Serial.println(_storageAddress);
  Serial.print(F("t1StartEnergy "));
  Serial.println(_zone.t1StartEnergy, 6);
  Serial.print(F("t2StartEnergy "));
  Serial.println(_zone.t2StartEnergy, 6);
  Serial.print(F("t1EnergyAcc "));
  Serial.println(_zone.t1EnergyAcc, 6);
  Serial.print(F("t2EnergyAcc "));
  Serial.println(_zone.t2EnergyAcc, 6);
}

// Protected

void BasePzem::calcZoneEnergy() {
  float t1Energy;
  float t2Energy;

  if (isT1ZoneActive(_createdAt.hour)) {
    t1Energy = calcT1ZoneEnergy();
    t2Energy = _zone.t2EnergyAcc;
  } else {
    t1Energy = _zone.t1EnergyAcc;
    t2Energy = calcT2ZoneEnergy();
  }

  _t1Energy = t1Energy;
  _t2Energy = t2Energy;
}

// Private

float BasePzem::calcT1ZoneEnergy() {
  bool isStartOfZone =
      isStartOfT1Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);
  bool isEndOfZone =
      isEndOfT1Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);

  if ((!_zone.t1StartEnergy && _energy > 0) || isStartOfZone) {
    _zone.t1StartEnergy = _energy;

    saveZone();
  }

  float t1Energy = _energy - _zone.t1StartEnergy + _zone.t1EnergyAcc;

  if (isEndOfZone) {
    _zone.t1EnergyAcc = t1Energy;

    saveZone();
  }

  return t1Energy;
}

float BasePzem::calcT2ZoneEnergy() {
  bool isStartOfZone =
      isStartOfT2Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);
  bool isEndOfZone =
      isEndOfT2Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);

  if ((!_zone.t2StartEnergy && _energy > 0) || isStartOfZone) {
    _zone.t2StartEnergy = _energy;

    saveZone();
  }

  float t2Energy = _energy - _zone.t2StartEnergy + _zone.t2EnergyAcc;

  if (isEndOfZone) {
    _zone.t2EnergyAcc = t2Energy;

    saveZone();
  }

  return t2Energy;
}

bool BasePzem::isT1ZoneActive(uint8_t hour) {
  return hour >= 7 && hour < 23;
}

bool BasePzem::isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 7 && minute == 0 && second == 0;
}

bool BasePzem::isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 22 && minute == 59 && second == 59;
}

bool BasePzem::isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 23 && minute == 0 && second == 0;
}

bool BasePzem::isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 6 && minute == 59 && second == 59;
}

void BasePzem::clearZone() {
  _zone.t1StartEnergy = 0.0;
  _zone.t2StartEnergy = 0.0;
  _zone.t1EnergyAcc = 0.0;
  _zone.t2EnergyAcc = 0.0;

  saveZone();
}

/**
 * Note: Zone struct consists of 4 float variables
 * This means that the Zone size is 4 * 4 bytes = 16 bytes
 * Need to take this into account to set proper addresses
 */
void BasePzem::saveZone() {
  Serial.println(F("Save zone"));

  Serial.print(F("Zone data to save to: "));
  Serial.println(_storageAddress);
  Serial.print(F("t1StartEnergy "));
  Serial.println(_zone.t1StartEnergy, 6);
  Serial.print(F("t2StartEnergy "));
  Serial.println(_zone.t2StartEnergy, 6);
  Serial.print(F("t1EnergyAcc "));
  Serial.println(_zone.t1EnergyAcc, 6);
  Serial.print(F("t2EnergyAcc "));
  Serial.println(_zone.t2EnergyAcc, 6);

  putValue(_storageAddress, _zone);
}

Zone BasePzem::getZone() {
  Zone zone;

  bool isSuccess = getValue(_storageAddress, zone);

  if (!isSuccess) {
    return zone;
  }

  if (isnan(zone.t1StartEnergy)) {
    zone.t1StartEnergy = 0.0;
  }

  if (isnan(zone.t2StartEnergy)) {
    zone.t2StartEnergy = 0.0;
  }

  if (isnan(zone.t1EnergyAcc)) {
    zone.t1EnergyAcc = 0.0;
  }

  if (isnan(zone.t2EnergyAcc)) {
    zone.t2EnergyAcc = 0.0;
  }

  return zone;
}