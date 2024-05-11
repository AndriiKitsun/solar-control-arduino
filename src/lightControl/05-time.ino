GyverNTP ntp(0);

void startNTP() {
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);
}

void syncTime() {
  ntp.tick();
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
