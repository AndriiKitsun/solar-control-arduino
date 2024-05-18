SoftwareSerial pzemSerial(PZEM_TX_PIN, PZEM_RX_PIN);
PZEM004Tv30 pzem(pzemSerial);

JsonDocument getPzemValues() {
  JsonDocument doc;

  doc["voltageV"] = pzem.voltage();
  doc["currentA"] = pzem.current();
  doc["powerKw"] = pzem.power() / 1000.0;
  doc["energyKwh"] = pzem.energy();
  doc["frequencyHz"] = pzem.frequency();
  doc["powerFactor"] = pzem.pf();

  doc["creationTimeGmt"] = getISODateTimeString();

  return doc;
}

String getJsonPzemValues() {
  String result;

  serializeJson(getPzemValues(), result);

  return result;
}

void resetEnergyCounter() {
  pzem.resetEnergy();
}