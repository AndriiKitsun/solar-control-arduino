#pragma once
#ifndef EEPROM_H
#define EEPROM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <at24c256.h>

static AT24C256 eeprom(AT24C_ADDRESS_0);

void startEeprom();
bool isEepromConnected();
JsonDocument getEepromStatus();

template <typename T>
bool putValue(uint8_t address, const T& data) {
  eeprom.put(address, data);

  uint8_t error = eeprom.getLastError();

  if (error != 0) {
    Serial.print(F("Error putting to EEPROM: "));
    Serial.println(error);
  }

  return error == 0;
}

template <typename T>
bool getValue(uint8_t address, T& data) {
  eeprom.get(address, data);

  uint8_t error = eeprom.getLastError();

  if (error != 0) {
    Serial.print(F("Error getting from EEPROM: "));
    Serial.println(error);
  }

  return error == 0;
}

#endif
