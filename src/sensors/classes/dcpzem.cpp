#include "sensors/classes/dcpzem.h"

// Public

DcPzem::DcPzem(String name, uint8_t roPin, uint8_t reDePin, uint8_t diPin, uint8_t pzemAddress)
    : BasePzem(name), _roPin(roPin), _reDePin(reDePin), _diPin(diPin), _pzemAddress(pzemAddress) {}

void DcPzem::startPzem() {
  _pzemSerial.begin(PZEM_BAUD_RATE, SWSERIAL_8N2, _roPin, _diPin);

  initPin(_reDePin, LOW);

  _node.preTransmission(_preTransmissionCb);
  _node.postTransmission(_postTransmissionCb);
  _node.begin(_pzemAddress, _pzemSerial);
}

void DcPzem::preTransmission() {
  pinHigh(_reDePin);
  delay(1);
}

void DcPzem::postTransmission() {
  delay(3);
  pinLow(_reDePin);
}

JsonDocument DcPzem::getStatus() {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("isConnected")] = isConnected();

  if (_node.readHoldingRegisters(REG_READ_START, HOLDING_REG_COUNT) == _node.ku8MBSuccess) {
    doc[F("currentAddress")] = _pzemAddress;
    doc[F("savedAddress")] = _node.getResponseBuffer(HOLDING_REG_ADDRESS);
    doc[F("savedShuntType")] = _node.getResponseBuffer(HOLDING_REG_SHUNT);
  }

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

  if (!doc.isNull()) {
    doc[F("name")] = _name;
  }

  return doc;
}

JsonDocument DcPzem::changeAddress(uint8_t addr) {
  JsonDocument doc;

  doc[F("name")] = _name;
  doc[F("addressToSet")] = addr;

  // Sanity check
  if (addr < 0x01 || addr > 0xF7) {
    doc[F("isChanged")] = false;

    return doc;
  }

  uint8_t result = _node.writeSingleRegister(HOLDING_REG_ADDRESS, addr);

  doc[F("isChanged")] = result == _node.ku8MBSuccess;

  return doc;
}

JsonDocument DcPzem::changeShuntType(uint8_t type) {
  JsonDocument doc;

  // Sanity check
  if (type < 0) {
    type = 0;
  }

  if (type > 3) {
    type = 3;
  }

  uint8_t result = _node.writeSingleRegister(HOLDING_REG_SHUNT, type);

  doc[F("name")] = _name;
  doc[F("shuntTypeToSet")] = type;
  doc[F("isChanged")] = result == _node.ku8MBSuccess;

  return doc;
}

JsonDocument DcPzem::resetCounter() {
  JsonDocument doc;

  doc[F("name")] = _name;

  if (!isConnected()) {
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

  doc[F("isReset")] = isSuccess;

  return doc;
}

// Private

bool DcPzem::isConnected() {
  return _voltage || _current || _power || _energy;
}

void DcPzem::readValues() {
  if (_node.readInputRegisters(REG_READ_START, INPUT_REG_COUNT) == _node.ku8MBSuccess) {
    _voltage = _node.getResponseBuffer(INPUT_REG_VOLTAGE) / 100.0;                                                         // Raw Voltage, V
    _current = _node.getResponseBuffer(INPUT_REG_CURRENT) / 100.0;                                                         // Raw Current, A
    _power = ((_node.getResponseBuffer(INPUT_REG_POWER_H) << 16) + _node.getResponseBuffer(INPUT_REG_POWER_L)) / 10000.0;  // Raw power, kW
    _energy = (_node.getResponseBuffer(INPUT_REG_ENERGY_H) << 16) + _node.getResponseBuffer(INPUT_REG_ENERGY_L) / 1000.0;  // Raw energy, kWh
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
