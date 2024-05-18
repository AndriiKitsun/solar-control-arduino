GyverNTP ntp(0);

float t1StartEnergy = 0;
float t1EndEnergy = 0;
float t2StartEnergy = 0;
float t2EndEnergy = 0;

float t1Energy = 0;
float t2Energy = 0;

void startNTP() {
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);
}

void syncTime() {
  ntp.tick();
}

uint8_t updateTime() {
  return ntp.updateNow();
}

String getNTPStatus() {
  JsonDocument doc;
  String result;

  doc["status"] = ntp.status();
  doc["isSynced"] = ntp.synced();
  doc["isBusy"] = ntp.busy();
  doc["ping"] = ntp.ping();
  doc["isoDate"] = getISODateTimeString();
  doc["tickStatus"] = ntp.tick();

  serializeJson(doc, result);

  return result;
}

String getNTPStatusWithUpdateStatus(uint8_t updateStatus) {
  JsonDocument doc;
  String result;

  doc["status"] = ntp.status();
  doc["isSynced"] = ntp.synced();
  doc["isBusy"] = ntp.busy();
  doc["ping"] = ntp.ping();
  doc["isoDate"] = getISODateTimeString();
  doc["tickStatus"] = ntp.tick();
  doc["updateStatus"] = updateStatus;

  serializeJson(doc, result);

  return result;
}

String getISODateTimeString() {
  String str;
  str.reserve(24);

  str += ntp.year();
  str += "-";

  if (ntp.month() < 10) {
    str += "0";
  }
  str += ntp.month();
  str += "-";

  if (ntp.day() < 10) {
    str += "0";
  }
  str += ntp.day();

  str += "T";

  if (ntp.hour() < 10) {
    str += "0";
  }
  str += ntp.hour();
  str += ":";

  if (ntp.minute() < 10) {
    str += "0";
  }
  str += ntp.minute();
  str += ":";

  if (ntp.second() < 10) {
    str += "0";
  }
  str += ntp.second();
  str += ".";

  if (ntp.ms() < 100) {
    str += "0";
  }
  str += ntp.ms();
  str += "Z";

  return str;
}

uint8_t getCurrentHour() {
  return ntp.hour();
}

uint8_t getCurrentDay() {
  return ntp.day();
}

bool isT1ZoneNow() {
  return ntp.hour() >= 7 && ntp.hour() < 23;
}

bool isT2ZoneNow() {
  return ntp.hour() == 23 || ntp.hour() == 0 || ntp.hour() < 7;
}

void calcZoneEnergy() {
  float energy = pzem.energy();

  // 7:00 UTC+3 == 4:00 UTC
  // 23:00 UTC+3 == 20:00 UTC
  if ((ntp.hour() == 7 && ntp.minute() == 0 && ntp.second == 0)) {
    t1StartEnergy = energy;
    t2EndEnergy = energy;
  }

  if (ntp.hour() == 23 && ntp.minute() == 0 && ntp.second == 0) {
    t2StartEnergy = energy;
    t1EndEnergy = energy;
  }

  if (!t1StartEnergy) {
    t1StartEnergy = energy;
  }

  if (!t2StartEnergy) {
    t2StartEnergy = energy;
  }

  t1Energy = isT1ZoneNow() ? energy - t1StartEnergy : t1EndEnergy - t1StartEnergy;
  t2Energy = isT2ZoneNow() ? energy - t2StartEnergy : t2EndEnergy - t2StartEnergy
}