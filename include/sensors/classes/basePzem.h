#pragma once
#ifndef BASE_PZEM_H
#define BASE_PZEM_H

#include <Arduino.h>

class BasePzem {
 public:
  explicit BasePzem(String name);

 protected:
  String _name;

  float _voltage;

  // Default mode
  float _current;
  float _power;
  float _energy;

  // Apparent mode
  float _apparentCurrent;
  float _apparentPower;
  float _apparentEnergy;

  // reactive mode
  float _reactiveCurrent;
  float _reactivePower;
  float _reactiveEnergy;

  // our mode
  float _ourReactivePower;
  float _ourApparentPower;
  float _ourApparentPowerSum;
};

#endif
