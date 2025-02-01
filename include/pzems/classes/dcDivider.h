#pragma once
#ifndef DC_DIVIDER_H
#define DC_DIVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "basePzem.h"
#include "utils/pin.h"

#define VREF 3.284      // Internal ESP voltage
#define DIV_R1 94510.0  // 94 K
#define DIV_R2 5000.0   // 5K

class DcDivider {
 public:
  DcDivider(String name);

  void begin();
  JsonDocument getValues();

 private:
  String _name;

  float getVoltage();
};

#endif
