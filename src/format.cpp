#include "format.h"

#include <string>

using std::string;

#define SECONDS2HOURS (3600)
#define SECONDS2MIN (60)

string Format::ElapsedTime(long int seconds) {
  string formated_hour;
  string formated_min;
  string formated_sec;
  string formated_time;

  int hour = seconds / SECONDS2HOURS;
  if (hour < 10) {
    formated_hour = "0" + std::to_string(hour);
  } else {
    formated_hour = std::to_string(hour);
  }

  int minute = (seconds / SECONDS2MIN) % SECONDS2MIN;
  if (minute < 10) {
    formated_min = "0" + std::to_string(minute);
  } else {
    formated_min = std::to_string(minute);
  }

  int second = seconds % SECONDS2MIN;
  if (second < 10) {
    formated_sec = "0" + std::to_string(second);
  } else {
    formated_sec = std::to_string(second);
  }

  formated_time = formated_hour + ":" + formated_min + ":" + formated_sec;

  return formated_time;
}