#pragma once
#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <Arduino.h>

class MovingAverage {
 private:
  uint16_t _windowSize;
  float* _values;
  int _index;
  int _count;
  float _sum;

 public:
  MovingAverage(uint16_t size);

  ~MovingAverage();

  void addValue(float value);
  float getAverage();
  void reset();
};

#endif
