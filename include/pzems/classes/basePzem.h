#pragma once
#ifndef BASE_PZEM_H
#define BASE_PZEM_H

#include <Arduino.h>

class BasePzem {
 public:
  BasePzem(String name);

 protected:
  String _name;

  float _voltage;
  float _current;
  float _power;
  float _energy;
};

#endif
