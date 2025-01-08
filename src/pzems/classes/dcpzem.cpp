#include "pzems/classes/dcpzem.h"

// Public

DcPzem::DcPzem(String name, uint8_t roPin, uint8_t reDePin, uint8_t diPin, uint8_t storageAddress, uint8_t pzemAddress)
    : BasePzem(name, storageAddress), _roPin(roPin), _reDePin(reDePin), _diPin(diPin), _pzemAddress(pzemAddress) {}

void DcPzem::startPzem() {
  _pzemSerial.begin(PZEM_BAUD_RATE, SWSERIAL_8N2, _roPin, _diPin);

  pinMode(_reDePin, OUTPUT);
  digitalWrite(_reDePin, LOW);

  _node.preTransmission(_preTransmissionCb);
  _node.postTransmission(_postTransmissionCb);
  _node.begin(_pzemAddress, _pzemSerial);

  BasePzem::startPzem();
}

void DcPzem::preTransmission() {
  digitalWrite(_reDePin, HIGH);
  delay(1);
}

void DcPzem::postTransmission() {
  delay(3);
  digitalWrite(_reDePin, LOW);
}

// TODO
JsonDocument DcPzem::getStatus() {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("isConnected")] = isConnected();
  // doc[F("currentAddress")] = _pzem.getAddress();
  // doc[F("savedAddress")] = _pzem.getHoldingAddress();
  // doc[F("savedShuntType")] = _pzem.getShuntType();

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

  if (!doc.isNull()) {
    doc[F("name")] = _name;
  }

  return doc;
}

// TODO
JsonDocument DcPzem::changeAddress(uint8_t addr) {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("addressToSet")] = addr;
  // doc[F("isChanged")] = _pzem.setAddress(addr);

  return doc;
}

// TODO
JsonDocument DcPzem::changeShuntType(uint16_t type) {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("shuntTypeToSet")] = type;
  // doc[F("isChanged")] = _pzem.setShuntType(type);

  return doc;
}

JsonDocument DcPzem::resetCounter() {
  JsonDocument doc;

  doc[F("name")] = _name;

  if (!isConnected() || !isEepromConnected()) {
    doc[F("isReset")] = false;

    return doc;
  }

  uint16_t u16CRC = 0xFFFF;
  uint8_t response[5];

  u16CRC = crc16_update(u16CRC, _pzemAddress);
  u16CRC = crc16_update(u16CRC, CMD_RESET);

  preTransmission();

  _pzemSerial.write(_pzemAddress);
  _pzemSerial.write(CMD_RESET);
  _pzemSerial.write(lowByte(u16CRC));
  _pzemSerial.write(highByte(u16CRC));

  postTransmission();

  uint8_t responseLength = readSerial(response, 5);

  bool isSuccess = !(responseLength == 0 || responseLength == 5);

  if (isSuccess) {
    clearZone();
  }

  doc[F("isReset")] = isSuccess;

  return doc;
}

// Private

bool DcPzem::isConnected() {
  return !isnan(_voltage) || !isnan(_current) || !isnan(_power) || !isnan(_energy);
}

void DcPzem::readValues() {
  uint8_t result;

  result = _node.readInputRegisters(0x0000, 5);

  if (result == _node.ku8MBSuccess) {
    _voltage = _node.getResponseBuffer(REG_VOLTAGE) / 100.0;                                                   // Raw Voltage, V
    _current = _node.getResponseBuffer(REG_CURRENT) / 100.0;                                                   // Raw Current, A
    _power = ((_node.getResponseBuffer(REG_POWER_H) << 16) + _node.getResponseBuffer(REG_POWER_L)) / 10000.0;  // Raw power, kW
    _energy = (_node.getResponseBuffer(REG_ENERGY_H) << 16) + _node.getResponseBuffer(REG_ENERGY_L) / 1000.0;  // Raw energy, kWh
  } else {
    _voltage = 0.0;
    _current = 0.0;
    _power = 0.0;
    _energy = 0.0;
  }
}

uint8_t DcPzem::readSerial(uint8_t* response, uint8_t length) {
  unsigned long startTime = millis();
  uint8_t index = 0;

  while ((index < length) && (millis() - startTime < READ_TIMEOUT)) {
    if (_pzemSerial.available()) {
      response[index++] = _pzemSerial.read();
    }

    yield();
  }

  return index;
}
