#include "pzems/classes/dcpzem.h"

// Public

DcPzem::DcPzem(SoftwareSerial& port, uint8_t storageAddress, uint8_t pzemAddress)
    : BasePzem(storageAddress), _pzem(port, pzemAddress) {}

JsonDocument DcPzem::getStatus() {
  JsonDocument doc;

  doc[F("isConnected")] = isConnected();
  doc[F("currentAddress")] = _pzem.getAddress();
  doc[F("savedAddress")] = _pzem.getHoldingAddress();
  doc[F("savedShuntType")] = _pzem.getShuntType();

  return doc;
}

JsonDocument DcPzem::getValues(const Date& date) {
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

  if (_t1Energy) {
    doc[F("t1EnergyKwh")] = _t1Energy;
  }

  if (_t2Energy) {
    doc[F("t2EnergyKwh")] = _t2Energy;
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

  doc[F("shuntTypeToSet")] = type;
  doc[F("isChanged")] = _pzem.setShuntType(type);

  return doc;
}

bool DcPzem::resetCounter() {
  if (!isConnected() || !isEepromConnected()) {
    return false;
  }

  bool isSuccess = _pzem.resetEnergy();

  if (isSuccess) {
    clearZone();
  }

  return isSuccess;
}

// Private

bool DcPzem::isConnected() {
  return !isnan(_pzem.voltage()) || !isnan(_pzem.current()) || !isnan(_pzem.power()) || !isnan(_pzem.energy());
}

void DcPzem::readValues() {
  _voltage = _pzem.voltage();

  if (DEBUG_MODE) {
    _current = _pzem.current();
    _power = _pzem.power() / 1000.0;
    _energy = _pzem.energy();
  } else {
    // If sensor is disconnected - clear response, skip further sensor polling to improve performance
    if (isnan(_voltage)) {
      _voltage = 0.0;
      _current = 0.0;
      _power = 0.0;
      _energy = 0.0;
      _t1Energy = 0.0;
      _t2Energy = 0.0;

      return;
    }

    _current = _pzem.current();
    _power = _pzem.power() / 1000.0;
    _energy = _pzem.energy();
  }

  if (!isnan(_energy)) {
    calcZoneEnergy();
  }
}
