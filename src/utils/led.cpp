#include "utils/led.h"

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void turnLedOn() {
  digitalWrite(LED_BUILTIN, LOW);
}

void turnLedOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
