#include "utils/eeprom.h"

void startEeprom() {
  Wire.begin();

  Serial.print(F("EEPROM module started. Size: "));
  Serial.print(eeprom.length());
  Serial.print(F(" Status: "));
  Serial.println(isEepromConnected() ? F("OK") : F("Disconnected"));
}

bool isEepromConnected() {
  bool response;

  eeprom.get(0, response);

  return eeprom.getLastError() == 0;
}

JsonDocument getEepromStatus() {
  JsonDocument doc;

  doc[F("isConnected")] = isEepromConnected();

  return doc;
}