#include "utils/moving-average.h"

// Public

MovingAverage::MovingAverage(uint8_t size) {
  _windowSize = size;
  _values = new float[_windowSize];
  _index = 0;
  _count = 0;
  _sum = 0;
}

MovingAverage::~MovingAverage() {
  delete[] _values;
}

void MovingAverage::addValue(float value) {
  if (_count == _windowSize) {
    _sum -= _values[_index];
  } else {
    _count++;
  }

  _values[_index] = value;
  _sum += value;

  _index = (_index + 1) % _windowSize;
}

float MovingAverage::getAverage() {
  if (_count < _windowSize) {
    return 0;
  }

  return _sum / _count;
}