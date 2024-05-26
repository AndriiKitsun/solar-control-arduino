GyverNTP ntp(TIMEZONE_OFFSET);

void startNTP() {
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);

  if (status > 1) {
    Serial.print(F("NTP Status is not ok. Updating... "));

    uint8_t status = updateTime();

    Serial.print(F("Status: "));
    Serial.println(status);
  }
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
  doc["isoDate"] = toISODateString(getDate());
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
  doc["isoDate"] = toISODateString(getDate());
  doc["tickStatus"] = ntp.tick();
  doc["updateStatus"] = updateStatus;

  serializeJson(doc, result);

  return result;
}

Date getDate() {
  return {
    year: ntp.year(),
    month: ntp.month(),
    day: ntp.day(),
    hour: ntp.hour(),
    minute: ntp.minute(),
    second: ntp.second(),
    ms: ntp.ms(),
  };
}

String toISODateString(Date date) {
  String str;
  str.reserve(24);

  str += date.year;
  str += "-";

  if (date.month < 10) {
    str += "0";
  }
  str += date.month;
  str += "-";

  if (date.day < 10) {
    str += "0";
  }
  str += date.day;

  str += "T";

  if (date.hour < 10) {
    str += "0";
  }
  str += date.hour;
  str += ":";

  if (date.minute < 10) {
    str += "0";
  }
  str += date.minute;
  str += ":";

  if (date.second < 10) {
    str += "0";
  }
  str += date.second;
  str += ".";

  if (date.ms < 100) {
    str += "0";
  }
  str += date.ms;
  str += "Z";

  return str;
}

bool isT1ZoneActive(uint8_t hour) {
  return hour >= 7 && hour < 23;
}

bool isStartOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 7 && minute == 0 && second == 0;
}

bool isEndOfT1Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 22 && minute == 59 && second == 59;
}

bool isStartOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 23 && minute == 0 && second == 0;
}

bool isEndOfT2Zone(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour == 6 && minute == 59 && second == 59;
}
