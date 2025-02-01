#include "sensors/classes/dcDivider.h"

// Public

DcDivider::DcDivider(String name)
    : _name(name) {
}

void DcDivider::start() {
  pinMode(A0, INPUT);
}

JsonDocument DcDivider::getValues() {
  JsonDocument doc;

  doc[F("voltage")] = getVoltage();
  doc[F("name")] = _name;

  return doc;
}

// Private

float DcDivider::getVoltage() {
  int value = analogRead(A0);
  float vOut = value * VREF / 1024;

  return vOut * ((DIV_R1 + DIV_R2) / DIV_R2);
}
