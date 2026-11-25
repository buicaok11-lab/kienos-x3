#pragma once
#include <Arduino.h>
#include "LunarCalendar.h"
#include "CultivationManager.h"

class SleepScreen528 {
public:
  static constexpr int WIDTH = 528;
  static constexpr int HEIGHT = 792;
  static constexpr int BUFFER_BYTES = (WIDTH * HEIGHT) / 8; // 52,272 bytes

  SleepScreen528();
  void clear(bool white = true);
  void setPixel(int x, int y, bool black);
  void drawRect(int x, int y, int w, int h, bool black = true);
  void fillRect(int x, int y, int w, int h, bool black = true);
  void drawProgressBar(int x, int y, int w, int h, int pct);

  // Vẽ toàn bộ giao diện Sleep Screen: Lịch Việt + Tu Tiên
  void render(const CultivationState& st, const LunarDate& lunar,
              int year, int month, int day, int weekdayIdx);

  const uint8_t* getBuffer() const { return _buf; }

private:
  uint8_t _buf[BUFFER_BYTES];
};
