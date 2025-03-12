#pragma once
#ifndef DC_DIVIDER_H
#define DC_DIVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "utils/moving-average.h"

class DcDivider {
 public:
  DcDivider(String name, uint16_t avgVoltageSize);

  void start();
  JsonDocument getValues();

 private:
  String _name;
  MovingAverage _avgVoltage;

  float getVoltage();
};

#endif
