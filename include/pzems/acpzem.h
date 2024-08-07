#pragma once
#ifndef ACPZEM_H
#define ACPZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include "utils/date.h"

struct Zone {
  float t1StartEnergy;
  float t2StartEnergy;
  float t1EnergyAcc;
  float t2EnergyAcc;
};

class AcPzem {
 public:
  AcPzem(SoftwareSerial& port, uint8_t addr = PZEM_DEFAULT_ADDR);

  JsonDocument getValues(const Date& date);
  JsonDocument changeAddress(uint8_t addr);
  void resetCounter();

 private:
  PZEM004Tv30 _pzem;
  Zone _zone;

  float _voltage;
  float _current;
  float _power;
  float _energy;
  float _frequency;
  float _powerFactor;
  float _t1Energy;
  float _t2Energy;
  Date _createdAt;

  void readValues();

  void calcZoneEnergy();

  float calcT1ZoneEnergy();
  float calcT2ZoneEnergy();

  bool isT1ZoneActive(uint8_t hour);

  bool isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second);
  bool isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second);

  bool isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second);
  bool isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second);
};

#endif
