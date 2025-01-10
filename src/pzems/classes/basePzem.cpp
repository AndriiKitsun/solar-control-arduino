#include "pzems/classes/basePzem.h"

// Public

BasePzem::BasePzem(String name)
    : _name(name) {
  _voltage = 0.0;
  _current = 0.0;
  _power = 0.0;
  _energy = 0.0;
}
