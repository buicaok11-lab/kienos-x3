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

  void drawText(int x, int y, const char* utf8Text, int scale = 2, bool bold = false);
  void drawTextRight(int rightX, int y, const char* utf8Text, int scale = 2, bool bold = false);
  void drawTextCentered(int centerX, int y, const char* utf8Text, int scale = 2, bool bold = false);
  int measureTextWidth(const char* utf8Text, int scale = 2) const;

  void render(const CultivationState& st, const LunarDate& lunar,
              int year, int month, int day, int weekdayIdx);

  // Xuất ảnh 1-bit BMP chuẩn 528x792 ra thẻ nhớ SD (sau khi đã gọi SD.begin)
  bool saveToBMP(const char* path = "/sleep_tutien.bmp") const;

  const uint8_t* getBuffer() const { return _buf; }

private:
  uint8_t _buf[BUFFER_BYTES];

  static uint32_t nextCodepoint(const char*& p);
  static char mapVietnameseToAscii(uint32_t cp);
  void drawChar8x16(int x, int y, char c, int scale, bool bold);
};
