#include "utils/date.h"

GyverNTP ntp(TIMEZONE_OFFSET);

void startNTP() {
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);

  if (status != GyverNTP::Status::OK) {
    Serial.print(F("NTP Status is not ok. Force updating... "));

    Serial.print(F("Update status: "));
    Serial.println(forceUpdate());
  }
}

bool forceUpdate() {
  return ntp.updateNow();
}

void tickNTP() {
  ntp.tick();
}

Date getDate() {
  return {
    year : ntp.year(),
    month : ntp.month(),
    day : ntp.day(),
    hour : ntp.hour(),
    minute : ntp.minute(),
    second : ntp.second(),
    ms : ntp.ms(),
  };
}

String getISODateString() {
  return toISODateString(getDate());
}

String toISODateString(const Date& date) {
  String str;
  str.reserve(24);

  str += date.year;
  str += F("-");

  if (date.month < 10) {
    str += F("0");
  }
  str += date.month;
  str += F("-");

  if (date.day < 10) {
    str += F("0");
  }
  str += date.day;

  str += F("T");

  if (date.hour < 10) {
    str += F("0");
  }
  str += date.hour;
  str += F(":");

  if (date.minute < 10) {
    str += F("0");
  }
  str += date.minute;
  str += F(":");

  if (date.second < 10) {
    str += F("0");
  }
  str += date.second;
  str += F(".");

  if (date.ms < 100) {
    str += F("0");
  }
  str += date.ms;
  str += F("Z");

  return str;
}
