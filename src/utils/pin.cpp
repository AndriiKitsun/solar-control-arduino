#include "utils/pin.h"

void initPin(uint8_t pin, uint8_t value) {
  pinMode(pin, OUTPUT);

  digitalWrite(pin, value);
}

void pinHigh(uint8_t pin) {
  digitalWrite(pin, HIGH);
}

void pinLow(uint8_t pin) {
  digitalWrite(pin, LOW);
}

void togglePin(uint8_t pin) {
  digitalWrite(pin, !digitalRead(pin));
}