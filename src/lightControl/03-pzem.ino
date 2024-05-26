SoftwareSerial pzemSerial(PZEM_TX_PIN, PZEM_RX_PIN);
PZEM004Tv30 pzem(pzemSerial);

float t1StartEnergy = 0;
float t2StartEnergy = 0;
float t1EnergyAcc = 0;
float t2EnergyAcc = 0;

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

    calcZonedEnergy(sensor);
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
}

void calcZonedEnergy(Pzem &sensor) {
  float t1Energy;
  float t2Energy;

  Serial.print(F("Current day: "));
  Serial.println(sensor.createdAt.day);
  Serial.print(F("Current hour: "));
  Serial.println(sensor.createdAt.hour);
  Serial.print(F("Current minute: "));
  Serial.println(sensor.createdAt.minute);
  Serial.print(F("Current second: "));
  Serial.println(sensor.createdAt.second);

  if (isT1ZoneActive(sensor.createdAt.hour)) {
    Serial.println(F("Active zone is T1"));

    t1Energy = calcT1ZoneEnergy(sensor);
    t2Energy = t2EnergyAcc;

    Serial.print(F("Calculated T1: "));
    Serial.println(t1Energy);
    Serial.print(F("T2 Acc value: "));
    Serial.println(t2EnergyAcc);
  } else {
    Serial.println(F("Active zone is T2"));

    t1Energy = t1EnergyAcc;
    t2Energy = calcT2ZoneEnergy(sensor);

    Serial.print(F("T1 Acc value: "));
    Serial.println(t1EnergyAcc);
    Serial.print(F("Calculated T2: "));
    Serial.println(t2Energy);
  }

  Serial.println(F("---- After calculation ----"));
  Serial.print(F("Calculated T1 energy: "));
  Serial.println(t1Energy);
  Serial.print(F("Calculated T2 energy: "));
  Serial.println(t2Energy);

  sensor.t1Energy = t1Energy;
  sensor.t2Energy = t2Energy;
}

float calcT1ZoneEnergy(Pzem sensor) {
  bool isStartOfZone = isStartOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  Serial.print(F("T1: isStartOfZone predicate: "));
  Serial.println(isStartOfZone);
  Serial.print(F("T1: isEndOfZone predicate: "));
  Serial.println(isEndOfZone);

  if (!t1StartEnergy || isStartOfZone) {
    t1StartEnergy = sensor.energy;
  }

  float t1Energy = sensor.energy - t1StartEnergy + t1EnergyAcc;

  Serial.println(F("T1: Operands"));
  Serial.print(F("T1: current energy: "));
  Serial.println(sensor.energy);
  Serial.print(F("T1: t1StartEnergy: "));
  Serial.println(t1StartEnergy);
  Serial.print(F("T1: t1EnergyAcc: "));
  Serial.println(t1EnergyAcc);

  if (isEndOfZone) {
    t1EnergyAcc = t1Energy;
  }

  return t1Energy;
}

float calcT2ZoneEnergy(Pzem sensor) {
  bool isStartOfZone = isStartOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  Serial.print(F("T2: isStartOfZone predicate: "));
  Serial.println(isStartOfZone);
  Serial.print(F("T2: isEndOfZone predicate: "));
  Serial.println(isEndOfZone);

  if (!t2StartEnergy || isStartOfZone) {
    t2StartEnergy = sensor.energy;
  }

  float t2Energy = sensor.energy - t2StartEnergy + t2EnergyAcc;

  Serial.println(F("T1: Operands"));
  Serial.print(F("T2: current energy: "));
  Serial.println(sensor.energy);
  Serial.print(F("T2: t2StartEnergy: "));
  Serial.println(t2StartEnergy);
  Serial.print(F("T2: t2EnergyAcc: "));
  Serial.println(t2EnergyAcc);

  if (isEndOfZone) {
    t2EnergyAcc = t2Energy;
  }

  return t2Energy;
}
