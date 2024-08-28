#pragma once
#ifndef DCPZEM_H
#define DCPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM017v1.h>
#include "pzem.h"

class DcPzem : public Pzem {
 public:
  DcPzem(SoftwareSerial& port, uint8_t storageAddress, uint8_t pzemAddress = PZEM017_DEFAULT_ADDR);

  JsonDocument getStatus();
  JsonDocument getValues(const Date& date);

  JsonDocument changeAddress(uint8_t addr);
  JsonDocument changeShuntType(uint16_t type);
  bool resetCounter();

 private:
  PZEM017v1 _pzem;

  bool isConnected();
  void readValues();
};

#endif
