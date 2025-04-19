#include "sensors/classes/acPzem.h"

// Public

AcPzem::AcPzem(String name, SoftwareSerial& port, uint8_t storageAddress, uint8_t pzemAddress, uint16_t avgVoltageSize)
    : _pzem(port, pzemAddress), _avgVoltageCalc(avgVoltageSize), _avgFrequencyCalc(AC_SENSOR_AVG_FREQUENCY_SIZE) {
  _name = name;
  _storageAddress = storageAddress;

  _avgVoltageIdlePeriod = avgVoltageSize * SENSORS_AVG_IDLE_PERIOD * 1000;
  _avgFrequencyIdlePeriod = AC_SENSOR_AVG_FREQUENCY_SIZE * SENSORS_AVG_IDLE_PERIOD * 1000;
}

void AcPzem::startPzem() {
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

JsonDocument AcPzem::getStatus() {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("isConnected")] = isConnected();
  doc[F("currentAddress")] = _pzem.getAddress();
  doc[F("savedAddress")] = _pzem.readAddress();

  return doc;
}

JsonDocument AcPzem::getValues(const Date& date) {
  JsonDocument doc;

  _createdAt = date;

  readValues();

  if (_voltage) {
    doc[F("voltage")] = _voltage;
  }

  if (_avgVoltage) {
    doc[F("avgVoltage")] = _avgVoltage;
  }

  if (_current) {
    doc[F("current")] = _current;
  }

  if (_power) {
    doc[F("power")] = _power;
  }

  if (_energy) {
    doc[F("energy")] = _energy;
  }

  if (_frequency) {
    doc[F("frequency")] = _frequency;
  }

  if (_avgFrequency) {
    doc[F("avgFrequency")] = _avgFrequency;
  }

  if (_powerFactor) {
    doc[F("powerFactor")] = _powerFactor;
  }

  if (_t1Energy) {
    doc[F("t1Energy")] = _t1Energy;
  }

  if (_t2Energy) {
    doc[F("t2Energy")] = _t2Energy;
  }

  if (doc.isNull()) {
    return doc;
  }

  doc[F("name")] = _name;

  return doc;
}

JsonDocument AcPzem::changeAddress(uint8_t addr) {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("addressToSet")] = addr;
  doc[F("isChanged")] = _pzem.setAddress(addr);

  return doc;
}

JsonDocument AcPzem::resetCounter() {
  JsonDocument doc;

  doc[F("name")] = _name;

  if (!isConnected() || !isEepromConnected()) {
    doc[F("isReset")] = false;

    return doc;
  }

  bool isSuccess = _pzem.resetEnergy();

  if (isSuccess) {
    clearZone();
  }

  doc[F("isReset")] = isSuccess;

  return doc;
}

// Private

bool AcPzem::isConnected() {
  return !isnan(_pzem.voltage());
}

void AcPzem::readValues() {
  _voltage = _pzem.voltage();

  calcAvgVoltage();

  // If sensor is disconnected - clear values and skip further sensor polling
  if (isnan(_voltage)) {
    _voltage = 0.0;
    _avgVoltage = 0.0;
    _current = 0.0;
    _power = 0.0;
    _energy = 0.0;
    _frequency = 0.0;
    _powerFactor = 0.0;
    _t1Energy = 0.0;
    _t2Energy = 0.0;

    return;
  }

  _powerFactor = _pzem.pf();
  _current = _pzem.current();
  _power = _pzem.power() / 1000.0;
  _energy = _pzem.energy();
  _frequency = _pzem.frequency();

  calcAvgFrequency();

  calcZoneEnergy();
}

void AcPzem::calcAvgVoltage() {
  if (!isnan(_voltage)) {
    _avgVoltageTimeoutMs = 0;

    _avgVoltageCalc.addValue(_voltage);

    _avgVoltage = _avgVoltageCalc.getAverage();

    return;
  }

  if (!_avgVoltageTimeoutMs) {
    _avgVoltageTimeoutMs = millis();
  }

  if (millis() - _avgVoltageTimeoutMs >= _avgVoltageIdlePeriod) {
    _avgVoltageTimeoutMs = millis();

    _avgVoltageCalc.reset();
  }
}

void AcPzem::calcAvgFrequency() {
  if (!isnan(_frequency)) {
    _avgFrequencyTimeoutMs = 0;

    _avgFrequencyCalc.addValue(_frequency);

    _avgFrequency = _avgFrequencyCalc.getAverage();

    return;
  }

  if (!_avgFrequencyTimeoutMs) {
    _avgFrequencyTimeoutMs = millis();
  }

  if (millis() - _avgFrequencyTimeoutMs >= _avgFrequencyIdlePeriod) {
    _avgFrequencyTimeoutMs = millis();

    _avgFrequencyCalc.reset();
  }
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

float AcPzem::calcT2ZoneEnergy() {
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

void AcPzem::clearZone() {
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
void AcPzem::saveZone() {
  putValue(_storageAddress, _zone);
}

Zone AcPzem::getZone() {
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
