#pragma once
#ifndef DCPZEM_H
#define DCPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ModbusMaster.h"
#include "SoftwareSerial.h"
#include "basePzem.h"

class DcPzem : public BasePzem {
 public:
  DcPzem(String name, uint8_t roPin, uint8_t reDePin, uint8_t diPin, uint8_t storageAddress, uint8_t pzemAddress);

  void (*_preTransmissionCb)();
  void (*_postTransmissionCb)();

  void startPzem();
  void preTransmission();
  void postTransmission();

  JsonDocument getStatus();
  JsonDocument getValues(const Date& date);

  JsonDocument changeAddress(uint8_t addr);
  JsonDocument changeShuntType(uint16_t type);
  JsonDocument resetCounter();

 private:
  ModbusMaster _node;
  SoftwareSerial _pzemSerial;
  uint8_t _roPin;
  uint8_t _reDePin;
  uint8_t _diPin;
  uint8_t _pzemAddress;

  bool isConnected();
  void readValues();
};

#endif
