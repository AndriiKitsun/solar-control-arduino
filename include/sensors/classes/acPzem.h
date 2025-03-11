#pragma once
#ifndef AC_PZEM_H
#define AC_PZEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include "utils/date.h"
#include "utils/eeprom.h"
#include "utils/moving-average.h"

// sizeof: 16
struct Zone {
  float t1StartEnergy = 0.0;
  float t2StartEnergy = 0.0;
  float t1EnergyAcc = 0.0;
  float t2EnergyAcc = 0.0;
};

class AcPzem {
 public:
  AcPzem(String name, SoftwareSerial& port, uint8_t storageAddress, uint8_t pzemAddress, uint8_t avgVoltageSize);

  void startPzem();

  JsonDocument getStatus();
  JsonDocument getValues(const Date& date);

  JsonDocument changeAddress(uint8_t addr);
  JsonDocument resetCounter();

 private:
  String _name;
  PZEM004Tv30 _pzem;
  Date _createdAt;
  Zone _zone;
  uint8_t _storageAddress;
  MovingAverage _avgVoltageCalc;

  float _voltage;
  float _avgVoltage;
  float _current;
  float _power;
  float _energy;
  float _frequency;
  float _powerFactor;
  float _t1Energy;
  float _t2Energy;

  bool isConnected();
  void readValues();
  void calcZoneEnergy();
  void clearZone();

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
