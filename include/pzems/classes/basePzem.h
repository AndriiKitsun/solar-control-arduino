#pragma once
#ifndef BASE_PZEM_H
#define BASE_PZEM_H

#include <Arduino.h>
#include "utils/date.h"
#include "utils/eeprom.h"

struct Zone {
  float t1StartEnergy = 0.0;
  float t2StartEnergy = 0.0;
  float t1EnergyAcc = 0.0;
  float t2EnergyAcc = 0.0;
};

class BasePzem {
 public:
  BasePzem(uint8_t storageAddress);

  void startPzem();

 protected:
  Date _createdAt;

  float _voltage;
  float _current;
  float _power;
  float _energy;
  float _t1Energy;
  float _t2Energy;

  void calcZoneEnergy();
  void clearZone();

 private:
  Zone _zone;
  uint8_t _storageAddress;

  float calcT1ZoneEnergy();
  float calcT2ZoneEnergy();

  bool isT1ZoneActive(uint8_t hour);
  bool isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second);
  bool isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second);
  bool isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second);
  bool isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second);

  void saveZone();
  Zone getZone();
};

#endif
