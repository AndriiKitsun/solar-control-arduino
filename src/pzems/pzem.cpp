#include "pzems/pzem.h"

// Protected

void Pzem::calcZoneEnergy() {
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

float Pzem::calcT1ZoneEnergy() {
  bool isStartOfZone =
      isStartOfT1Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);
  bool isEndOfZone =
      isEndOfT1Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);

  if (!_zone.t1StartEnergy || isStartOfZone) {
    _zone.t1StartEnergy = _energy;
  }

  float t1Energy = _energy - _zone.t1StartEnergy + _zone.t1EnergyAcc;

  if (isEndOfZone) {
    _zone.t1EnergyAcc = t1Energy;
  }

  return t1Energy;
}

float Pzem::calcT2ZoneEnergy() {
  bool isStartOfZone =
      isStartOfT2Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);
  bool isEndOfZone =
      isEndOfT2Zone(_createdAt.hour, _createdAt.minute, _createdAt.second);

  if (!_zone.t2StartEnergy || isStartOfZone) {
    _zone.t2StartEnergy = _energy;
  }

  float t2Energy = _energy - _zone.t2StartEnergy + _zone.t2EnergyAcc;

  if (isEndOfZone) {
    _zone.t2EnergyAcc = t2Energy;
  }

  return t2Energy;
}

bool Pzem::isT1ZoneActive(uint8_t hour) {
  return hour >= 7 && hour < 23;
}

bool Pzem::isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 7 && minute == 0 && second == 0;
}

bool Pzem::isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 22 && minute == 59 && second == 59;
}

bool Pzem::isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 23 && minute == 0 && second == 0;
}

bool Pzem::isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 6 && minute == 59 && second == 59;
}

void Pzem::saveZone(int address, const Zone& zone) {
  setValue(address, zone);
}

Zone Pzem::getZone(int address) {
  Zone zone;

  Serial.print("Address '");
  Serial.print(address);
  Serial.println("' -->");

  Serial.println("Default values: ");
  Serial.println(zone.t1StartEnergy);
  Serial.println(zone.t2StartEnergy);
  Serial.println(zone.t1EnergyAcc);
  Serial.println(zone.t2EnergyAcc);

  bool isSuccess = getValue(address, zone);

  if (!isSuccess) {
    Serial.println("Reading is not success. Return default values. Values from eeprom: ");
    Serial.println(zone.t1StartEnergy, 6);
    Serial.println(zone.t2StartEnergy, 6);
    Serial.println(zone.t1EnergyAcc, 6);
    Serial.println(zone.t2EnergyAcc, 6);

    return zone;
  }

  Serial.println("Read is ok. Values: ");
  Serial.println(zone.t1StartEnergy, 6);
  Serial.println(zone.t2StartEnergy, 6);
  Serial.println(zone.t1EnergyAcc, 6);
  Serial.println(zone.t2EnergyAcc, 6);

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

  Serial.println("After conditional transformation: ");
  Serial.println(zone.t1StartEnergy, 6);
  Serial.println(zone.t2StartEnergy, 6);
  Serial.println(zone.t1EnergyAcc, 6);
  Serial.println(zone.t2EnergyAcc, 6);

  Serial.println();

  return zone;
}