#include "utils/date.h"

static TimeChangeRule dstStart = {"EEST", Last, Sun, Mar, 3, 180};  // UTC+3
static TimeChangeRule stdStart = {"EET", Last, Sun, Oct, 4, 120};   // UTC+2
static Timezone uaTZ(dstStart, stdStart);
static TimeChangeRule* tcr;

void startNTP() {
  bool status = NTP.begin(0);

  Serial.print(F("NTP started. Status: "));
  Serial.println(status);

  if (!status) {
    Serial.print(F("NTP Status is not ok. Force updating... "));

    Serial.print(F("Update status: "));
    Serial.println(NTP.updateNow());
  }
}

void tickNTP() {
  NTP.tick();
}

Date getUTCDate() {
  Datime dt(NTP);

  return {
    year : dt.year,
    month : dt.month,
    day : dt.day,
    hour : dt.hour,
    minute : dt.minute,
    second : dt.second,
    ms : NTP.ms(),
  };
}

Date getLocalDate() {
  time_t localTime = uaTZ.toLocal(NTP.getUnix(), &tcr);

  return {
    year : year(localTime),
    month : month(localTime),
    day : day(localTime),
    hour : hour(localTime),
    minute : minute(localTime),
    second : second(localTime),
    ms : NTP.ms(),
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
