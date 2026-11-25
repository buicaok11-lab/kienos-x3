#include <Arduino.h>
#include "LunarCalendar.h"

void setup() {
  Serial.begin(115200);
  delay(200);

  LunarDate ld = LunarCalendar::solarToLunar(25, 11, 2026);
  Serial.printf("Am lich: %02d/%02d (%s) - Ngay %s\n",
                ld.day, ld.month, ld.yearCanChi.c_str(), ld.dayCanChi.c_str());
}

void loop() {
  delay(1000);
}
