#include "utils/date.h"

static GyverNTP ntp(0);

static TimeChangeRule dstStart = {"EEST", Last, Sun, Mar, 3, 180};  // UTC+3
static TimeChangeRule stdStart = {"EET", Last, Sun, Oct, 4, 120};   // UTC+2
static Timezone uaTZ(dstStart, stdStart);
static TimeChangeRule* tcr;

void startNTP() {
  bool status = ntp.begin();

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);

  if (status != GyverNTP::Status::OK) {
    Serial.print(F("NTP Status is not ok. Force updating... "));

    Serial.print(F("Update status: "));
    Serial.println(ntp.updateNow());
  }
}

void tickNTP() {
  ntp.tick();
}

Date getUTCDate() {
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

Date getLocalDate() {
  time_t localTime = uaTZ.toLocal(ntp.getUnix(), &tcr);

  return {
    year : year(localTime),
    month : month(localTime),
    day : day(localTime),
    hour : hour(localTime),
    minute : minute(localTime),
    second : second(localTime),
    ms : ntp.ms(),
  };
}

JsonDocument getDateStatus() {
  JsonDocument doc;

  doc[F("createdAtGmt")] = toJSON(getUTCDate());
  doc[F("createdAt")] = toJSON(getLocalDate());

  return doc;
}

String toJSON(const Date& date) {
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
