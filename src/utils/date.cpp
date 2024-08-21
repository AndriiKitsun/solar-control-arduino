#include "utils/date.h"

GyverNTP ntp(0);
RTC_DS3231 rtc;

TimeChangeRule dstStart = {"EEST", Last, Sun, Mar, 3, 180};  // UTC+3
TimeChangeRule stdStart = {"EET", Last, Sun, Oct, 4, 120};   // UTC+2
Timezone uaTZ(dstStart, stdStart);
TimeChangeRule* tcr;

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

void startRTC() {
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC!"));

    return;
  }

  Serial.println(F("RTC module started"));

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, setting the time from NTP"));

    rtc.adjust(DateTime(ntp.year(), ntp.month(), ntp.day(), ntp.hour(), ntp.minute(), ntp.second()));
  }
}

void tickNTP() {
  ntp.tick();
}

Date getNTPDate() {
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

Date getRTCDate() {
  DateTime now = rtc.now();

  return {
    year : now.year(),
    month : now.month(),
    day : now.day(),
    hour : now.hour(),
    minute : now.minute(),
    second : now.second(),
    ms : 0,
  };
}

Date getNTPLocalDate() {
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

Date getRTCLocalDate() {
  time_t localTime = uaTZ.toLocal(rtc.now().unixtime(), &tcr);

  return {
    year : year(localTime),
    month : month(localTime),
    day : day(localTime),
    hour : hour(localTime),
    minute : minute(localTime),
    second : second(localTime),
    ms : 0,
  };
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
  if (date.ms < 10) {
    str += F("0");
  }
  str += date.ms;
  str += F("Z");

  return str;
}
