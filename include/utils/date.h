#pragma once
#ifndef DATE_H
#define DATE_H

#include <Arduino.h>
#include <GyverNTP.h>

struct Date {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t ms;
};

void startNTP();
bool forceUpdate();
void tickNTP();

Date getDate();

String getISODateString();
String toISODateString(const Date& date);

#endif
