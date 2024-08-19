#pragma once
#ifndef DCPZEM_H
#define DCPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM017v1.h>

class DcPzem {
 public:
  DcPzem(SoftwareSerial& port, uint8_t addr = PZEM017_DEFAULT_ADDR);

  JsonDocument getStatus();
  JsonDocument getValues();

  JsonDocument changeAddress(uint8_t addr);
  JsonDocument changeShuntType(uint16_t type);
  void resetCounter();

 private:
  PZEM017v1 _pzem;

  float _voltage;
  float _current;
  float _power;
  float _energy;

  void readValues();
};

#endif
