SoftwareSerial pzemSerial(PZEM_TX_PIN, PZEM_RX_PIN);

// Pzem custom address can be between 0x01 (1) and 0xF7 (247)
PZEM004Tv30 inputPzem(pzemSerial, 0x01);
PZEM004Tv30 outputPzem(pzemSerial, 0x02);

Zone inputPzemZone = { 0.0, 0.0, 0.0, 0.0 };
Zone outputPzemZone = { 0.0, 0.0, 0.0, 0.0 };

Pzem getPzemValues(PZEM004Tv30 &pzem, Zone &zone) {
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

    calcZoneEnergy(sensor, zone);
  }

  return sensor;
}

JsonDocument pzemToJson(const Pzem &sensor) {
  JsonDocument doc;

  doc[F("voltageV")] = sensor.voltage;
  doc[F("currentA")] = sensor.current;
  doc[F("powerKw")] = sensor.power;
  doc[F("energyKwh")] = sensor.energy;
  doc[F("frequencyHz")] = sensor.frequency;
  doc[F("powerFactor")] = sensor.powerFactor;
  doc[F("t1EnergyKwh")] = sensor.t1Energy;
  doc[F("t2EnergyKwh")] = sensor.t2Energy;
  doc[F("createdAt")] = toISODateString(sensor.createdAt);

  return doc;
}

void calcZoneEnergy(Pzem &sensor, Zone &zone) {
  float t1Energy;
  float t2Energy;

  if (isT1ZoneActive(sensor.createdAt.hour)) {
    t1Energy = calcT1ZoneEnergy(sensor, zone);
    t2Energy = zone.t2EnergyAcc;
  } else {
    t1Energy = zone.t1EnergyAcc;
    t2Energy = calcT2ZoneEnergy(sensor, zone);
  }

  sensor.t1Energy = t1Energy;
  sensor.t2Energy = t2Energy;
}

float calcT1ZoneEnergy(const Pzem &sensor, Zone &zone) {
  bool isStartOfZone = isStartOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT1Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!zone.t1StartEnergy || isStartOfZone) {
    zone.t1StartEnergy = sensor.energy;
  }

  float t1Energy = sensor.energy - zone.t1StartEnergy + zone.t1EnergyAcc;

  if (isEndOfZone) {
    zone.t1EnergyAcc = t1Energy;
  }

  return t1Energy;
}

float calcT2ZoneEnergy(const Pzem &sensor, Zone &zone) {
  bool isStartOfZone = isStartOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);
  bool isEndOfZone = isEndOfT2Zone(sensor.createdAt.hour, sensor.createdAt.minute, sensor.createdAt.second);

  if (!zone.t2StartEnergy || isStartOfZone) {
    zone.t2StartEnergy = sensor.energy;
  }

  float t2Energy = sensor.energy - zone.t2StartEnergy + zone.t2EnergyAcc;

  if (isEndOfZone) {
    zone.t2EnergyAcc = t2Energy;
  }

  return t2Energy;
}

void resetPzemCounter(PZEM004Tv30 &sensor, Zone &zone) {
  sensor.resetEnergy();

  zone.t1StartEnergy = 0.0;
  zone.t2StartEnergy = 0.0;
  zone.t1EnergyAcc = 0.0;
  zone.t2EnergyAcc = 0.0;
}

void resetEnergyCounter() {
  resetPzemCounter(inputPzem, inputPzemZone);
  resetPzemCounter(outputPzem, outputPzemZone);
}

String collectPzemPayload() {
  JsonDocument doc;
  String payload;

  unsigned long start = millis();

  doc[F("inputAc")].to<JsonObject>();
  doc[F("outputAc")].to<JsonObject>();

  unsigned long addedObjects = millis();

  doc[F("inputAc")] = pzemToJson(getPzemValues(inputPzem, inputPzemZone));

  unsigned long firstCalc = millis();

  doc[F("outputAc")] = pzemToJson(getPzemValues(outputPzem, outputPzemZone));

  unsigned long secCalc = millis();

  serializeJson(doc, payload);

  unsigned long end = millis();

  Serial.print(F("Adding objects: "));
  Serial.println(addedObjects - start);
  Serial.print(F("Input Pzem calculation: "));
  Serial.println(firstCalc - addedObjects);
  Serial.print(F("Output Pzem calculation: "));
  Serial.println(secCalc - firstCalc);
  Serial.print(F("Calculating: "));
  Serial.println(secCalc - addedObjects);

  Serial.print(F("Whole method: "));
  Serial.println(end - start);

  return payload;
}
