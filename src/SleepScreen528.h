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

  // Vẽ chữ UTF-8 (hỗ trợ tiếng Việt, scale 1 = 8x16px, scale 2 = 16x32px, scale 3 = 24x48px)
  void drawText(int x, int y, const char* utf8Text, int scale = 2, bool bold = false);
  void drawTextRight(int rightX, int y, const char* utf8Text, int scale = 2, bool bold = false);
  void drawTextCentered(int centerX, int y, const char* utf8Text, int scale = 2, bool bold = false);
  int measureTextWidth(const char* utf8Text, int scale = 2) const;

  // Vẽ toàn bộ giao diện Sleep Screen: Lịch Việt + Tu Tiên
  void render(const CultivationState& st, const LunarDate& lunar,
              int year, int month, int day, int weekdayIdx);

  const uint8_t* getBuffer() const { return _buf; }

private:
  uint8_t _buf[BUFFER_BYTES];

  // Đọc từng codepoint Unicode từ chuỗi UTF-8
  static uint32_t nextCodepoint(const char*& p);
  // Chuẩn hóa codepoint tiếng Việt sang ASCII cơ bản để dựng nét gọn nhẹ trên RAM nhỏ
  static char mapVietnameseToAscii(uint32_t cp);
  void drawChar8x16(int x, int y, char c, int scale, bool bold);
};
