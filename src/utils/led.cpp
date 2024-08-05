#include "utils/led.h"

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void turnLedOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
