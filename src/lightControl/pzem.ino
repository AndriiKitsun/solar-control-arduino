SoftwareSerial pzemSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSerial);

String getPzemValues() {
  String result = "Pzem values:\n\n";
  result.reserve(500);

  unsigned long startTime = millis();
  unsigned long endTime;
  unsigned long elapsedTime;

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  endTime = millis();
  elapsedTime = endTime - startTime;

  result += "Voltage: " + String(voltage) + " V\n";
  result += "Current: " + String(current) + " A\n";
  result += "Power: " + String(power) + " W\n";
  result += "Energy: " + String(energy) + " kWh\n";
  result += "Frequency: " + String(frequency) + " Hz\n";
  result += "PF: " + String(pf) + "\n";
  result += "Start time: " + String(startTime) + " ms\n";
  result += "End time: " + String(endTime) + " ms\n";
  result += "Elapsed time: " + String(elapsedTime) + " ms\n";

  return result;
}

void printPzemValues() {
  // Read the data from the sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("Energy: ");
  Serial.print(energy);
  Serial.println(" kWh");

  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  Serial.print("PF: ");
  Serial.println(pf);
}