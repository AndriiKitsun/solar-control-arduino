SoftwareSerial pzemSerial(PZEM_TX_PIN, PZEM_RX_PIN);
PZEM004Tv30 pzem(pzemSerial);

float t1StartEnergy = 0.0;
float t2StartEnergy = 0.0;
float t1EnergyAcc = 0.0;
float t2EnergyAcc = 0.0;

Pzem getPzemValues() {
  Pzem sensor;

  sensor.voltage = pzem.voltage();
  sensor.createdAt = getDate();

  if (isnan(sensor.voltage)) {
    sensor.current = NAN;
    sensor.power = NAN;
    sensor.energy = NAN;
    sensor.frequency = NAN;
    sensor.powerFactor = NAN;
    sensor.t1Energy = NAN;
    sensor.t2Energy = NAN;
  } else {
    sensor.current = pzem.current();
    sensor.power = pzem.power() / 1000.0;
    sensor.energy = pzem.energy();
    sensor.frequency = pzem.frequency();
    sensor.powerFactor = pzem.pf();

    calcZoneEnergy(sensor);
  }

  return sensor;
}

String pzemToJson(Pzem sensor) {
  JsonDocument doc;
  String result;

  doc["voltageV"] = sensor.voltage;
  doc["currentA"] = sensor.current;
  doc["powerKw"] = sensor.power;
  doc["energyKwh"] = sensor.energy;
  doc["frequencyHz"] = sensor.frequency;
  doc["powerFactor"] = sensor.powerFactor;
  doc["t1EnergyKwh"] = sensor.t1Energy;
  doc["t2EnergyKwh"] = sensor.t2Energy;
  doc["createdAt"] = toISODateString(sensor.createdAt);

  serializeJson(doc, result);

  return result;
}

void resetEnergyCounter() {
  pzem.resetEnergy();

  t1StartEnergy = 0.0;
  t2StartEnergy = 0.0;
  t1EnergyAcc = 0.0;
  t2EnergyAcc = 0.0;
}

void calcZoneEnergy(Pzem &sensor) {
  float t1Energy;
  float t2Energy;

  if (isT1ZoneActive(sensor.createdAt.hour)) {
    t1Energy = calcT1ZoneEnergy(sensor);
    t2Energy = t2EnergyAcc;
  } else {
    t1Energy = t1EnergyAcc;
    t2Energy = calcT2ZoneEnergy(sensor);
  }

  sensor.t1Energy = t1Energy;
  sensor.t2Energy = t2Energy;
}

float calcT1ZoneEnergy(Pzem sensor) {
  bool isStartOfZone = isStartOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!t1StartEnergy || isStartOfZone) {
    t1StartEnergy = sensor.energy;
  }

  float t1Energy = sensor.energy - t1StartEnergy + t1EnergyAcc;

  if (isEndOfZone) {
    t1EnergyAcc = t1Energy;
  }

  return t1Energy;
}

float calcT2ZoneEnergy(Pzem sensor) {
  bool isStartOfZone = isStartOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!t2StartEnergy || isStartOfZone) {
    t2StartEnergy = sensor.energy;
  }

  float t2Energy = sensor.energy - t2StartEnergy + t2EnergyAcc;

  if (isEndOfZone) {
    t2EnergyAcc = t2Energy;
  }

  return t2Energy;
}
