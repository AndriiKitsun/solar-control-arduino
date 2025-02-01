#include "pzems/classes/dcDivider.h"

// Public

DcDivider::DcDivider(String name)
    : _name(name) {
}

void DcDivider::start() {
  pinMode(A0, INPUT);
}

JsonDocument DcDivider::getValues() {
  JsonDocument doc;

  readValues();

  if (_voltage) {
    doc[F("voltageV")] = _voltage;
  }

  if (!doc.isNull()) {
    doc[F("name")] = _name;
  }

  return doc;
}

// Private

void DcDivider::readValues() {
  int value = analogRead(A0);
  float vOut = value * VREF / 1024;

  _voltage = vOut * ((DIV_R1 + DIV_R2) / DIV_R2);
}
