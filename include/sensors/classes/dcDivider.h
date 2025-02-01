#pragma once
#ifndef DC_DIVIDER_H
#define DC_DIVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define VREF 3.284      // Internal ESP voltage
#define DIV_R1 97550.0  // 97K
#define DIV_R2 5000.0   // 5K

class DcDivider {
 public:
  DcDivider(String name);

  void start();
  JsonDocument getValues();

 private:
  String _name;

  float getVoltage();
};

#endif
