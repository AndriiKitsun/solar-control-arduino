#pragma once
#ifndef ACPZEM_H
#define ACPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include "pzem.h"

class AcPzem : public Pzem {
 public:
  AcPzem(SoftwareSerial& port, uint8_t addr = PZEM_DEFAULT_ADDR);

  JsonDocument getStatus();
  JsonDocument getValues(const Date& date);

  JsonDocument changeAddress(uint8_t addr);
  void resetCounter();

 private:
  PZEM004Tv30 _pzem;

  float _frequency;
  float _powerFactor;

  void readValues();
};

#endif
