SoftwareSerial pzemSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSerial);

JsonDocument getPzemValues() {
  JsonDocument doc;

  doc["voltage"] = pzem.voltage();
  doc["current"] = pzem.current();
  doc["power"] = pzem.power();
  doc["energy"] = pzem.energy();
  doc["frequency"] = pzem.frequency();
  doc["pf"] = pzem.pf();

  return doc;
}