#pragma once
#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

void initPin(uint8_t pin, uint8_t value = LOW);

void pinHigh(uint8_t pin);
void pinLow(uint8_t pin);
void togglePin(uint8_t pin);

#endif
