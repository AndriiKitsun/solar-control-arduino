PZEM017v1 dcPzem(DC_PZEM_RX_PIN, DC_PZEM_TX_PIN);

DcPzem getPzemValues(PZEM017v1 &pzem) {
  DcPzem sensor;

  sensor.voltage = pzem.voltage();
  sensor.current = pzem.current();
  sensor.power = pzem.power();
  sensor.energy = pzem.energy();

  sensor.address = pzem.getAddress();
  sensor.shuntType = pzem.getShunttype();
  sensor.updated = pzem.getParameters();

  sensor.createdAt = getDate();

  return sensor;
}

JsonDocument pzemToJson(const DcPzem &sensor) {
  JsonDocument doc;

  doc[F("voltageV")] = sensor.voltage;
  doc[F("currentA")] = sensor.current;
  doc[F("powerKw")] = sensor.power;
  doc[F("energyKwh")] = sensor.energy;

  doc[F("address")] = sensor.address;
  doc[F("shuntType")] = sensor.shuntType;
  doc[F("isUpdated")] = sensor.updated;

  doc[F("createdAt")] = toISODateString(sensor.createdAt);

  return doc;
}

String collectPzemPayload() {
  JsonDocument doc;
  String payload;

  doc[F("dcPzem")].to<JsonObject>();

  doc[F("dcPzem")] = pzemToJson(getPzemValues(dcPzem));

  serializeJson(doc, payload);

  return payload;
}

void setShunt() {
  dcPzem.setShuntType(SHUNT_50A);

  if (dcPzem.getAddress() != PZEM_DEFAULT_ADDR) {
    dcPzem.setAddress(PZEM_DEFAULT_ADDR);
  }
}
