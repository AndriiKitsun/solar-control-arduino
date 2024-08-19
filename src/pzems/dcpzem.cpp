#include "pzems/dcpzem.h"

// Public

DcPzem::DcPzem(SoftwareSerial& port, uint8_t addr)
    : _pzem(port, addr) {
}

JsonDocument DcPzem::getStatus() {
  JsonDocument doc;

  doc[F("isConnected")] = !isnan(_pzem.voltage());
  doc[F("address")] = _pzem.getAddress();
  doc[F("holdingAddress")] = _pzem.getHoldingAddress();
  doc[F("shuntType")] = _pzem.getShunttype();

  return doc;
}

JsonDocument DcPzem::getValues() {
  JsonDocument doc;

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

  return doc;
}

JsonDocument DcPzem::changeAddress(uint8_t addr) {
  JsonDocument doc;

  doc[F("addressToSet")] = addr;
  doc[F("isChanged")] = _pzem.setAddress(addr);

  return doc;
}

JsonDocument DcPzem::changeShuntType(uint16_t type) {
  JsonDocument doc;

  doc[F("shuntToSet")] = type;
  doc[F("isChanged")] = _pzem.setShuntType(type);

  return doc;
}

void DcPzem::resetCounter() {
  _pzem.resetEnergy();
}

// Private

void DcPzem::readValues() {
  _voltage = _pzem.voltage();

  // If sensor is disconnected - clear values and skip further sensor polling
  if (isnan(_voltage)) {
    _voltage = 0.0;
    _current = 0.0;
    _power = 0.0;
    _energy = 0.0;

    return;
  }

  _current = _pzem.current();
  _power = _pzem.power() / 1000.0;
  _energy = _pzem.energy();
}
