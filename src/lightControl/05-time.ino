GyverNTP ntp(0);

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
