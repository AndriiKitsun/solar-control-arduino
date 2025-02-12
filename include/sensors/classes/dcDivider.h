#pragma once
#ifndef DC_DIVIDER_H
#define DC_DIVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>

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
