#pragma once
#ifndef ACPZEM_H
#define ACPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include "basePzem.h"

class AcPzem : public BasePzem {
 public:
  AcPzem(String id, SoftwareSerial& port, uint8_t storageAddress, uint8_t pzemAddress = PZEM_DEFAULT_ADDR);

  JsonDocument getStatus();
  JsonDocument getValues(const Date& date);

  JsonDocument changeAddress(uint8_t addr);
  JsonDocument resetCounter();

 private:
  PZEM004Tv30 _pzem;

  float _frequency;
  float _powerFactor;

  bool isConnected();
  void readValues();
};

#endif
