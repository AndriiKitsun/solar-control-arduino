#pragma once
#ifndef DATE_H
#define DATE_H

#include <Arduino.h>
#include <GyverNTP.h>
#include <Timezone.h>

struct Date {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  uint16_t ms;
};

void startNTP();
void tickNTP();

Date getUTCDate();
Date getLocalDate();

String toJSON(const Date& date);

#endif
