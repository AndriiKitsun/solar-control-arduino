#pragma once
#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <Arduino.h>

class MovingAverage {
 private:
  uint8_t _windowSize;
  float* _values;
  int _index;
  int _count;
  float _sum;

 public:
  MovingAverage(uint8_t size);

  ~MovingAverage();

  void addValue(float value);

  float getAverage();
};

#endif
