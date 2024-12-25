#include "utils/relay.h"

void initRelayPin(uint8_t pin, uint8_t value) {
  pinMode(pin, OUTPUT);

  digitalWrite(pin, value);
}

void turnRelayOn(uint8_t pin) {
  digitalWrite(pin, HIGH);
}

void turnRelayOff(uint8_t pin) {
  digitalWrite(pin, LOW);
}

void toggleRelay(uint8_t pin) {
  digitalWrite(pin, !digitalRead(pin));
}