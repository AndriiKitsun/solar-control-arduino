#include "pzems/acpzem.h"

// Public

AcPzem::AcPzem(SoftwareSerial& port, uint8_t addr)
    : _pzem(port, addr) {
  _zone = {0.0, 0.0, 0.0, 0.0};
}

JsonDocument AcPzem::getStatus() {
  JsonDocument doc;

  doc[F("isConnected")] = !isnan(_pzem.voltage());
  doc[F("currentAddress")] = _pzem.getAddress();
  doc[F("savedAddress")] = _pzem.readAddress();

  return doc;
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

  if (_power) {
    doc[F("powerKw")] = _power;
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

  // If sensor is disconnected - clear values and skip further sensor polling
  if (isnan(_voltage)) {
    _voltage = 0.0;
    _current = 0.0;
    _power = 0.0;
    _energy = 0.0;
    _frequency = 0.0;
    _powerFactor = 0.0;
    _t1Energy = 0.0;
    _t2Energy = 0.0;

    return;
  }

  _current = _pzem.current();
  _power = _pzem.power() / 1000.0;
  _energy = _pzem.energy();
  _frequency = _pzem.frequency();
  _powerFactor = _pzem.pf();

  calcZoneEnergy();
}
