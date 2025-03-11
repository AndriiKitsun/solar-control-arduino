#include "sensors/classes/dcDivider.h"

// Public

DcDivider::DcDivider(String name, uint8_t avgVoltageSize)
    : _name(name), _avgVoltage(avgVoltageSize) {
}

void DcDivider::start() {
  pinMode(A0, INPUT);
}

JsonDocument DcDivider::getValues() {
  JsonDocument doc;
  float voltage = getVoltage();

  _avgVoltage.addValue(voltage);

  doc[F("voltage")] = voltage;
  doc[F("avgVoltage")] = _avgVoltage.getAverage();
  doc[F("name")] = _name;

  return doc;
}

// Private

float DcDivider::getVoltage() {
  int value = analogRead(A0);
  float vOut = value * DC_DIVIDER_VREF / 1024;

  return vOut * ((DC_DIVIDER_R1 + DC_DIVIDER_R2) / DC_DIVIDER_R2);
}
