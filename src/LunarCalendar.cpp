#include "LunarCalendar.h"
#include <cmath>

namespace {
  const char* CAN[] = {"Giáp", "Ất", "Bính", "Đinh", "Mậu", "Kỷ", "Canh", "Tân", "Nhâm", "Quý"};
  const char* CHI[] = {"Tý", "Sửu", "Dần", "Mão", "Thìn", "Tỵ", "Ngọ", "Mùi", "Thân", "Dậu", "Tuất", "Hợi"};

  int jdFromDate(int dd, int mm, int yy) {
    int a = (14 - mm) / 12;
    int y = yy + 4800 - a;
    int m = mm + 12 * a - 3;
    int jd = dd + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    if (jd < 2299161) {
      jd = dd + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083;
    }
    return jd;
  }

  int getNewMoonDay(int k, double timeZone) {
    double T = k / 1236.85;
    double T2 = T * T;
    double T3 = T2 * T;
    double dr = 3.14159265358979323846 / 180.0;
    double Jd1 = 2415020.75933 + 29.53058868 * k + 0.0001178 * T2 - 0.000000155 * T3;
    Jd1 += 0.00033 * sin((166.56 + 132.87 * T - 0.009173 * T2) * dr);
    double M = 359.2242 + 29.10535608 * k - 0.0000333 * T2 - 0.00000347 * T3;
    double Mpr = 306.0253 + 385.81691806 * k + 0.0107306 * T2 + 0.00001236 * T3;
    double F = 21.2964 + 390.67050646 * k - 0.0016528 * T2 - 0.00000239 * T3;
    double C1 = (0.1734 - 0.000393 * T) * sin(M * dr) + 0.0021 * sin(2 * dr * M);
    C1 = C1 - 0.4068 * sin(Mpr * dr) + 0.0161 * sin(dr * 2 * Mpr);
    C1 = C1 - 0.0004 * sin(dr * 3 * Mpr);
    C1 = C1 + 0.0104 * sin(dr * 2 * F) - 0.0051 * sin(dr * (M + Mpr));
    C1 = C1 - 0.0074 * sin(dr * (M - Mpr)) + 0.0004 * sin(dr * (2 * F + M));
    C1 = C1 - 0.0004 * sin(dr * (2 * F - M)) - 0.0006 * sin(dr * (2 * F + Mpr));
    C1 = C1 + 0.0010 * sin(dr * (2 * F - Mpr)) + 0.0005 * sin(dr * (2 * Mpr + M));
    double deltat;
    if (T < -11) {
      deltat = 0.001 + 0.000839 * T + 0.0002261 * T2 - 0.00000845 * T3 - 0.000000081 * T * T3;
    } else {
      deltat = -0.000278 + 0.000265 * T + 0.000262 * T2;
    }
    double JdNew = Jd1 + C1 - deltat;
    return (int)floor(JdNew + 0.5 + timeZone / 24.0);
  }

  double getSunLongitude(int dayNumber, double timeZone) {
    double dr = 3.14159265358979323846 / 180.0;
    double T = (dayNumber - 2451545.5 - timeZone / 24.0) / 36525.0;
    double T2 = T * T;
    double M = 357.52910 + 35999.05030 * T - 0.0001559 * T2 - 0.00000048 * T * T2;
    double L0 = 280.46645 + 36000.76983 * T + 0.0003032 * T2;
    double DL = (1.91460 - 0.004817 * T - 0.000014 * T2) * sin(dr * M);
    DL += (0.01993 - 0.000101 * T) * sin(dr * 2 * M) + 0.000290 * sin(dr * 3 * M);
    double L = (L0 + DL) * dr;
    L = L - 2 * 3.14159265358979323846 * floor(L / (2 * 3.14159265358979323846));
    return floor(L / (3.14159265358979323846 / 6.0));
  }

  int getLunarMonth11(int yy, double timeZone) {
    int off = jdFromDate(31, 12, yy) - 2415021;
    int k = (int)floor(off / 29.530588853);
    int nm = getNewMoonDay(k, timeZone);
    int sunLong = (int)getSunLongitude(nm, timeZone);
    if (sunLong >= 9) {
      nm = getNewMoonDay(k - 1, timeZone);
    }
    return nm;
  }

  int getLeapMonthOffset(int a11, double timeZone) {
    int k = (int)floor((a11 - 2415021.076998695) / 29.530588853 + 0.5);
    int last = 0;
    int i = 1;
    int arc = (int)getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone);
    do {
      last = arc;
      i++;
      arc = (int)getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone);
    } while (arc != last && i < 14);
    return i - 1;
  }
}

String LunarCalendar::getYearCanChi(int lunarYear) {
  int canIdx = (lunarYear + 6) % 10;
  int chiIdx = (lunarYear + 8) % 12;
  if (canIdx < 0) canIdx += 10;
  if (chiIdx < 0) chiIdx += 12;
  return String(CAN[canIdx]) + " " + String(CHI[chiIdx]);
}

String LunarCalendar::getDayCanChi(int jd) {
  int canIdx = (jd + 9) % 10;
  int chiIdx = (jd + 1) % 12;
  if (canIdx < 0) canIdx += 10;
  if (chiIdx < 0) chiIdx += 12;
  return String(CAN[canIdx]) + " " + String(CHI[chiIdx]);
}

LunarDate LunarCalendar::solarToLunar(int dd, int mm, int yyyy) {
  const double tz = 7.0; // Giờ Việt Nam UTC+7
  int dayNumber = jdFromDate(dd, mm, yyyy);
  int k = (int)floor((dayNumber - 2415021.076998695) / 29.530588853);
  int monthStart = getNewMoonDay(k + 1, tz);
  if (monthStart > dayNumber) {
    monthStart = getNewMoonDay(k, tz);
  }
  int a11 = getLunarMonth11(yyyy, tz);
  int b11 = a11;
  int lunarYear;
  if (a11 >= monthStart) {
    lunarYear = yyyy;
    a11 = getLunarMonth11(yyyy - 1, tz);
  } else {
    lunarYear = yyyy + 1;
    b11 = getLunarMonth11(yyyy + 1, tz);
  }
  int lunarDay = dayNumber - monthStart + 1;
  int diff = (monthStart - a11) / 29;
  bool lunarLeap = false;
  int lunarMonth = diff + 11;
  if (b11 - a11 > 365) {
    int leapMonthDiff = getLeapMonthOffset(a11, tz);
    if (diff >= leapMonthDiff) {
      lunarMonth = diff + 10;
      if (diff == leapMonthDiff) {
        lunarLeap = true;
      }
    }
  }
  if (lunarMonth > 12) {
    lunarMonth = lunarMonth - 12;
  }
  if (lunarMonth >= 11 && diff < 4) {
    lunarYear -= 1;
  }

  LunarDate res;
  res.day = lunarDay;
  res.month = lunarMonth;
  res.year = lunarYear;
  res.isLeap = lunarLeap;
  res.yearCanChi = getYearCanChi(lunarYear);
  res.dayCanChi = getDayCanChi(dayNumber);
  return res;
}
