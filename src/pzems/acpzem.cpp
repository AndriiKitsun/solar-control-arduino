#include "pzems/acpzem.h"

// Public

AcPzem::AcPzem(SoftwareSerial& port, uint8_t addr)
    : _pzem(port, addr), _zone{0.0, 0.0, 0.0, 0.0} {
}

JsonDocument AcPzem::getValues(const Date& date) {
  JsonDocument doc;

  _createdAt = date;

  readValues();

  if (_voltage) {
    doc[F("voltageV")] = _voltage;
  }

  if (_current) {
    doc[F("currentA")] = _current;
  }

  if (_energy) {
    doc[F("energyKwh")] = _energy;
  }

  if (_frequency) {
    doc[F("frequencyHz")] = _frequency;
  }

  if (_powerFactor) {
    doc[F("powerFactor")] = _powerFactor;
  }

  if (_t1Energy) {
    doc[F("t1EnergyKwh")] = _t1Energy;
  }

  if (_t2Energy) {
    doc[F("t2EnergyKwh")] = _t2Energy;
  }

  return doc;
}

JsonDocument AcPzem::changeAddress(uint8_t addr) {
  JsonDocument doc;

  doc[F("currentAddress")] = _pzem.getAddress();
  doc[F("addressToSet")] = addr;
  doc[F("isConnected")] = !isnan(_pzem.voltage());
  doc[F("isChanged")] = _pzem.setAddress(addr);

  return doc;
}

void AcPzem::resetCounter() {
  _pzem.resetEnergy();

  _zone.t1StartEnergy = 0.0;
  _zone.t2StartEnergy = 0.0;
  _zone.t1EnergyAcc = 0.0;
  _zone.t2EnergyAcc = 0.0;
}

// Private

void AcPzem::readValues() {
  _voltage = _pzem.voltage();

  // If the value can't be read - skip further sensor polling
  if (isnan(_voltage)) {
    _voltage = 0;
    return;
  }

  _current = _pzem.current();
  _power = _pzem.power() / 1000.0;
  _energy = _pzem.energy();
  _frequency = _pzem.frequency();
  _powerFactor = _pzem.pf();

  calcZoneEnergy();
}

void AcPzem::calcZoneEnergy() {
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

float AcPzem::calcT1ZoneEnergy() {
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

float AcPzem::calcT2ZoneEnergy() {
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

bool AcPzem::isT1ZoneActive(uint8_t hour) {
  return hour >= 7 && hour < 23;
}

bool AcPzem::isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 7 && minute == 0 && second == 0;
}

bool AcPzem::isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 22 && minute == 59 && second == 59;
}

bool AcPzem::isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 23 && minute == 0 && second == 0;
}

bool AcPzem::isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 6 && minute == 59 && second == 59;
}
