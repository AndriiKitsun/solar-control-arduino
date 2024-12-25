#pragma once
#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

void initRelayPin(uint8_t pin, uint8_t value = LOW);
void turnRelayOn(uint8_t pin);
void turnRelayOff(uint8_t pin);
void toggleRelay(uint8_t pin);

#endif
