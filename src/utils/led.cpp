#include "utils/led.h"

void initLedPins() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}