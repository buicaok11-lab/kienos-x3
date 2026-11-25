#pragma once
#include <Arduino.h>

struct LunarDate {
  int day;
  int month;
  int year;
  bool isLeap;
  String yearCanChi;
  String dayCanChi;

  // Ngày mùng 1 hoặc Rằm (15) âm lịch -> nhân đôi linh thạch
  bool isDoubleSpiritStoneDay() const {
    return (day == 1 || day == 15);
  }
};

class LunarCalendar {
public:
  // Chuyển đổi ngày dương lịch sang âm lịch Việt Nam (UTC+7)
  static LunarDate solarToLunar(int dd, int mm, int yyyy);
  static String getYearCanChi(int lunarYear);
  static String getDayCanChi(int jd);
};
