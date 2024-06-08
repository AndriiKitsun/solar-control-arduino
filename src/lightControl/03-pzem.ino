SoftwareSerial pzemSWSerial(PZEM_TX_PIN, PZEM_RX_PIN);

PZEM004Tv30 inputAcPzem(pzemSWSerial, INPUT_AC_PZEM_ADDRESS);
PZEM004Tv30 outputAcPzem(pzemSWSerial, OUTPUT_AC_PZEM_ADDRESS);

Zone inputAcPzemZone = { 0.0, 0.0, 0.0, 0.0 };
Zone outputAcPzemZone = { 0.0, 0.0, 0.0, 0.0 };

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

String changePzemAddress(uint8_t addr) {
  JsonDocument doc;
  String result;

  doc[F("currentAddress")] = inputAcPzem.getAddress();
  doc[F("addressToSet")] = addr;
  doc[F("isConnected")] = !isnan(inputAcPzem.voltage());
  doc[F("isChanged")] = inputAcPzem.setAddress(addr);

  serializeJson(doc, result);

  return result;
}

void resetPzemCounter(PZEM004Tv30 &sensor, Zone &zone) {
  sensor.resetEnergy();

  zone.t1StartEnergy = 0.0;
  zone.t2StartEnergy = 0.0;
  zone.t1EnergyAcc = 0.0;
  zone.t2EnergyAcc = 0.0;
}

void resetEnergyCounter() {
  resetPzemCounter(inputAcPzem, inputAcPzemZone);
  resetPzemCounter(outputAcPzem, outputAcPzemZone);
}

String collectPzemPayload() {
  JsonDocument doc;
  String payload;

  doc[F("inputAc")].to<JsonObject>();
  doc[F("outputAc")].to<JsonObject>();

  doc[F("inputAc")] = pzemToJson(getPzemValues(inputAcPzem, inputAcPzemZone));
  doc[F("outputAc")] = pzemToJson(getPzemValues(outputAcPzem, outputAcPzemZone));

  serializeJson(doc, payload);

  return payload;
}
