#pragma once
#ifndef PZEM_H
#define PZEM_H

#include <Arduino.h>
#include "utils/date.h"

struct Zone {
  float t1StartEnergy;
  float t2StartEnergy;
  float t1EnergyAcc;
  float t2EnergyAcc;
};

class Pzem {
 protected:
  Zone _zone;
  Date _createdAt;

  float _voltage;
  float _current;
  float _power;
  float _energy;

  float _t1Energy;
  float _t2Energy;

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
