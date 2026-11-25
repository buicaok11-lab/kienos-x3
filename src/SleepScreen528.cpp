#include "SleepScreen528.h"
#include <cstring>
#include <cstdio>

namespace {
  // Bảng ký tự 8x16 (mỗi ký tự 16 byte, bit cao nhất ở bên trái)
  const uint8_t FONT_8X16_BASIC[96][16] = {
    // 32: ' '
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    // 33: '!'
    {0,0,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0,0,0x18,0x18,0,0,0},
    // 34: '"'
    {0,0,0x66,0x66,0x24,0,0,0,0,0,0,0,0,0,0,0},
    // 35: '#'
    {0,0,0x24,0x24,0x7E,0x24,0x24,0x7E,0x24,0x24,0,0,0,0,0,0},
    // 36: '$'
    {0,0,0x08,0x3E,0x48,0x3E,0x0A,0x7C,0x08,0,0,0,0,0,0,0},
    // 37: '%'
    {0,0,0x62,0x64,0x08,0x10,0x26,0x46,0,0,0,0,0,0,0,0},
    // 38: '&'
    {0,0,0x38,0x44,0x44,0x38,0x6A,0x44,0x3A,0,0,0,0,0,0,0},
    // 39: '\''
    {0,0,0x18,0x18,0x30,0,0,0,0,0,0,0,0,0,0,0},
    // 40: '('
    {0,0,0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0,0,0,0,0,0,0},
    // 41: ')'
    {0,0,0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0,0,0,0,0,0,0},
    // 42: '*'
    {0,0,0,0x66,0x3C,0xFF,0x3C,0x66,0,0,0,0,0,0,0,0},
    // 43: '+'
    {0,0,0,0x18,0x18,0x7E,0x18,0x18,0,0,0,0,0,0,0,0},
    // 44: ','
    {0,0,0,0,0,0,0,0,0,0x18,0x18,0x30,0,0,0,0},
    // 45: '-'
    {0,0,0,0,0,0,0x7E,0,0,0,0,0,0,0,0,0},
    // 46: '.'
    {0,0,0,0,0,0,0,0,0,0,0x18,0x18,0,0,0,0},
    // 47: '/'
    {0,0,0x02,0x04,0x08,0x10,0x20,0x40,0,0,0,0,0,0,0,0},
    // 48..57: '0'..'9'
    {0,0,0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x06,0x1C,0x30,0x60,0x7E,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0,0,0,0,0,0,0},
    {0,0,0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0,0,0,0,0,0,0},
    // 58: ':'
    {0,0,0,0x18,0x18,0,0,0x18,0x18,0,0,0,0,0,0,0},
    // 59: ';'
    {0,0,0,0x18,0x18,0,0,0x18,0x18,0x30,0,0,0,0,0,0},
    // 60: '<'
    {0,0,0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0,0,0,0,0,0,0},
    // 61: '='
    {0,0,0,0,0x7E,0,0,0x7E,0,0,0,0,0,0,0,0},
    // 62: '>'
    {0,0,0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0,0,0,0,0,0,0},
    // 63: '?'
    {0,0,0x3C,0x66,0x06,0x0C,0x18,0,0x18,0,0,0,0,0,0,0},
    // 64: '@'
    {0,0,0x3C,0x66,0x6E,0x6E,0x6E,0x60,0x3E,0,0,0,0,0,0,0},
    // 65..90: 'A'..'Z'
    {0,0,0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0,0,0,0,0,0,0},
    {0,0,0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0,0,0,0,0,0,0},
    {0,0,0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x60,0x6E,0x66,0x66,0x3E,0,0,0,0,0,0,0},
    {0,0,0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0,0,0,0,0,0,0},
    {0,0,0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0,0,0,0,0,0,0},
    {0,0,0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0,0,0,0,0,0,0},
    {0,0,0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0,0,0,0,0,0,0},
    {0,0,0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0,0,0,0,0,0,0},
    {0,0,0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x66,0x66,0x6A,0x6C,0x36,0,0,0,0,0,0,0},
    {0,0,0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0,0,0,0,0,0,0},
    {0,0,0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0,0,0,0,0,0,0},
    {0,0,0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0,0,0,0,0,0,0},
    {0,0,0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0,0,0,0,0,0,0},
    {0,0,0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0,0,0,0,0,0,0},
    // 91..96: '[', '\\', ']', '^', '_', '`'
    {0,0,0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0,0,0,0,0,0,0},
    {0,0,0x40,0x20,0x10,0x08,0x04,0x02,0,0,0,0,0,0,0,0},
    {0,0,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0,0,0,0,0,0,0},
    {0,0,0x10,0x38,0x6C,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0xFF,0,0,0,0},
    {0,0,0x18,0x18,0x0C,0,0,0,0,0,0,0,0,0,0,0},
    // 97..122: 'a'..'z'
    {0,0,0,0,0x3C,0x06,0x3E,0x66,0x3E,0,0,0,0,0,0,0},
    {0,0,0x60,0x60,0x7C,0x66,0x66,0x66,0x7C,0,0,0,0,0,0,0},
    {0,0,0,0,0x3C,0x66,0x60,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0,0,0,0,0,0,0},
    {0,0,0,0,0x3C,0x66,0x7E,0x60,0x3C,0,0,0,0,0,0,0},
    {0,0,0x1C,0x30,0x7C,0x30,0x30,0x30,0x30,0,0,0,0,0,0,0},
    {0,0,0,0,0x3E,0x66,0x66,0x3E,0x06,0x3C,0,0,0,0,0,0},
    {0,0,0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0x18,0,0x38,0x18,0x18,0x18,0x3C,0,0,0,0,0,0,0},
    {0,0,0x0C,0,0x1C,0x0C,0x0C,0x0C,0x6C,0x38,0,0,0,0,0,0},
    {0,0,0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0,0,0,0,0,0,0},
    {0,0,0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0,0,0,0,0,0,0},
    {0,0,0,0,0x66,0x7F,0x7F,0x6B,0x63,0,0,0,0,0,0,0},
    {0,0,0,0,0x7C,0x66,0x66,0x66,0x66,0,0,0,0,0,0,0},
    {0,0,0,0,0x3C,0x66,0x66,0x66,0x3C,0,0,0,0,0,0,0},
    {0,0,0,0,0x7C,0x66,0x66,0x7C,0x60,0x60,0,0,0,0,0,0},
    {0,0,0,0,0x3E,0x66,0x66,0x3E,0x06,0x06,0,0,0,0,0,0},
    {0,0,0,0,0x7C,0x66,0x60,0x60,0x60,0,0,0,0,0,0,0},
    {0,0,0,0,0x3E,0x60,0x3C,0x06,0x7C,0,0,0,0,0,0,0},
    {0,0,0x18,0x18,0x7E,0x18,0x18,0x18,0x0E,0,0,0,0,0,0,0},
    {0,0,0,0,0x66,0x66,0x66,0x66,0x3E,0,0,0,0,0,0,0},
    {0,0,0,0,0x66,0x66,0x66,0x3C,0x18,0,0,0,0,0,0,0},
    {0,0,0,0,0x63,0x6B,0x7F,0x7F,0x63,0,0,0,0,0,0,0},
    {0,0,0,0,0x66,0x3C,0x18,0x3C,0x66,0,0,0,0,0,0,0},
    {0,0,0,0,0x66,0x66,0x66,0x3E,0x06,0x3C,0,0,0,0,0,0},
    {0,0,0,0,0x7E,0x0C,0x18,0x30,0x7E,0,0,0,0,0,0,0},
    // 123..127: '{', '|', '}', '~', DEL
    {0,0,0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0,0,0,0,0,0,0},
    {0,0,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0,0,0,0,0,0,0},
    {0,0,0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0,0,0,0,0,0,0},
    {0,0,0x32,0x4A,0x04,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };
}

SleepScreen528::SleepScreen528() {
  clear(true);
}

void SleepScreen528::clear(bool white) {
  memset(_buf, white ? 0xFF : 0x00, BUFFER_BYTES);
}

void SleepScreen528::setPixel(int x, int y, bool black) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  int idx = y * (WIDTH / 8) + (x / 8);
  uint8_t mask = (uint8_t)(0x80 >> (x % 8));
  if (black) {
    _buf[idx] &= ~mask;
  } else {
    _buf[idx] |= mask;
  }
}

void SleepScreen528::drawRect(int x, int y, int w, int h, bool black) {
  for (int i = 0; i < w; i++) {
    setPixel(x + i, y, black);
    setPixel(x + i, y + h - 1, black);
  }
  for (int j = 0; j < h; j++) {
    setPixel(x, y + j, black);
    setPixel(x + w - 1, y + j, black);
  }
}

void SleepScreen528::fillRect(int x, int y, int w, int h, bool black) {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      setPixel(x + i, y + j, black);
    }
  }
}

void SleepScreen528::drawProgressBar(int x, int y, int w, int h, int pct) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  drawRect(x, y, w, h, true);
  int innerW = ((w - 4) * pct) / 100;
  if (innerW > 0 && h > 4) {
    fillRect(x + 2, y + 2, innerW, h - 4, true);
  }
}

uint32_t SleepScreen528::nextCodepoint(const char*& p) {
  const uint8_t* u = (const uint8_t*)p;
  if (*u == 0) return 0;
  if (*u < 0x80) {
    p += 1;
    return u[0];
  }
  if ((*u & 0xE0) == 0xC0 && u[1]) {
    uint32_t cp = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
    p += 2;
    return cp;
  }
  if ((*u & 0xF0) == 0xE0 && u[1] && u[2]) {
    uint32_t cp = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F);
    p += 3;
    return cp;
  }
  p += 1;
  return '?';
}

char SleepScreen528::mapVietnameseToAscii(uint32_t cp) {
  if (cp >= 32 && cp <= 127) return (char)cp;
  if (cp == 0x00B7) return '.'; // '·'
  if (cp == 0x0110) return 'D'; // 'Đ'
  if (cp == 0x0111) return 'd'; // 'đ'

  // Nhóm A / a có dấu tiếng Việt
  if ((cp >= 0x00C0 && cp <= 0x00C3) || cp == 0x0102 || cp == 0x1EA0 ||
      (cp >= 0x1EA2 && cp <= 0x1EB7)) return 'A';
  if ((cp >= 0x00E0 && cp <= 0x00E3) || cp == 0x0103 || cp == 0x1EA1 ||
      (cp >= 0x1EA3 && cp <= 0x1EB7)) return 'a';

  // Nhóm E / e
  if ((cp >= 0x00C8 && cp <= 0x00CA) || cp == 0x1EB8 || (cp >= 0x1EBA && cp <= 0x1EC7)) return 'E';
  if ((cp >= 0x00E8 && cp <= 0x00EA) || cp == 0x1EB9 || (cp >= 0x1EBB && cp <= 0x1EC7)) return 'e';

  // Nhóm I / i
  if (cp == 0x00CC || cp == 0x00CD || cp == 0x0128 || cp == 0x1EC8 || cp == 0x1ECA) return 'I';
  if (cp == 0x00EC || cp == 0x00ED || cp == 0x0129 || cp == 0x1EC9 || cp == 0x1ECB) return 'i';

  // Nhóm O / o (bao gồm Ô, Ơ)
  if ((cp >= 0x00D2 && cp <= 0x00D5) || cp == 0x01A0 ||
      (cp >= 0x1ECC && cp <= 0x1EE3)) return 'O';
  if ((cp >= 0x00F2 && cp <= 0x00F5) || cp == 0x01A1 ||
      (cp >= 0x1ECD && cp <= 0x1EE3)) return 'o';

  // Nhóm U / u (bao gồm Ư)
  if (cp == 0x00D9 || cp == 0x00DA || cp == 0x0168 || cp == 0x01AF ||
      (cp >= 0x1EE4 && cp <= 0x1EF1)) return 'U';
  if (cp == 0x00F9 || cp == 0x00FA || cp == 0x0169 || cp == 0x01B0 ||
      (cp >= 0x1EE5 && cp <= 0x1EF1)) return 'u';

  // Nhóm Y / y
  if (cp == 0x00DD || (cp >= 0x1EF2 && cp <= 0x1EF9)) return 'Y';
  if (cp == 0x00FD || (cp >= 0x1EF3 && cp <= 0x1EF9)) return 'y';

  return '?';
}

void SleepScreen528::drawChar8x16(int x, int y, char c, int scale, bool bold) {
  int idx = (int)c - 32;
  if (idx < 0 || idx >= 96) idx = 31; // '?'
  const uint8_t* glyph = FONT_8X16_BASIC[idx];

  for (int row = 0; row < 16; row++) {
    uint8_t bits = glyph[row];
    for (int col = 0; col < 8; col++) {
      if (bits & (0x80 >> col)) {
        fillRect(x + col * scale, y + row * scale, scale, scale, true);
        if (bold) {
          fillRect(x + col * scale + 1, y + row * scale, scale, scale, true);
        }
      }
    }
  }
}

int SleepScreen528::measureTextWidth(const char* utf8Text, int scale) const {
  int count = 0;
  const char* p = utf8Text;
  while (nextCodepoint(p) != 0) {
    count++;
  }
  return count * (8 * scale);
}

void SleepScreen528::drawText(int x, int y, const char* utf8Text, int scale, bool bold) {
  const char* p = utf8Text;
  int cx = x;
  while (true) {
    uint32_t cp = nextCodepoint(p);
    if (cp == 0) break;
    char ch = mapVietnameseToAscii(cp);
    drawChar8x16(cx, y, ch, scale, bold);
    cx += 8 * scale + (bold ? 1 : 0);
  }
}

void SleepScreen528::drawTextRight(int rightX, int y, const char* utf8Text, int scale, bool bold) {
  int w = measureTextWidth(utf8Text, scale);
  drawText(rightX - w, y, utf8Text, scale, bold);
}

void SleepScreen528::drawTextCentered(int centerX, int y, const char* utf8Text, int scale, bool bold) {
  int w = measureTextWidth(utf8Text, scale);
  drawText(centerX - w / 2, y, utf8Text, scale, bold);
}

void SleepScreen528::render(const CultivationState& st, const LunarDate& lunar,
                           int year, int month, int day, int weekdayIdx) {
  clear(true);

  const int margin = 24;
  const int innerW = WIDTH - margin * 2; // 480
  char buf[96];

  // ================= 1. LỊCH DƯƠNG & LỊCH ÂM =================
  snprintf(buf, sizeof(buf), "THANG %d . %d", month, year);
  drawText(margin, 22, buf, 2, true);

  const char* weekdays[] = {"Chu nhat", "Thu hai", "Thu ba", "Thu tu", "Thu nam", "Thu sau", "Thu bay"};
  const char* wd = (weekdayIdx >= 0 && weekdayIdx < 7) ? weekdays[weekdayIdx] : "Thu tu";
  drawTextRight(WIDTH - margin, 22, wd, 2, true);

  // Số ngày dương lớn bên trái
  snprintf(buf, sizeof(buf), "%02d", day);
  drawText(margin + 48, 66, buf, 5, true);

  // Khối Âm lịch bên phải
  drawTextRight(WIDTH - margin, 64, "AM LICH", 1, true);
  snprintf(buf, sizeof(buf), "%02d/%02d", lunar.day, lunar.month);
  drawTextRight(WIDTH - margin, 84, buf, 3, true);

  snprintf(buf, sizeof(buf), "Nam %s", lunar.yearCanChi.c_str());
  drawTextRight(WIDTH - margin, 138, buf, 1, false);
  snprintf(buf, sizeof(buf), "Ngay %s", lunar.dayCanChi.c_str());
  drawTextRight(WIDTH - margin, 158, buf, 1, false);

  // Đường kẻ phân cách 1
  fillRect(margin, 204, innerW, 3, true);

  // ================= 2. CẢNH GIỚI & TU VI =================
  snprintf(buf, sizeof(buf), "%s . Tang %d",
           CultivationManager::getRealmName(st.realmIndex).c_str(), st.realmLayer);
  drawText(margin, 222, buf, 2, true);

  snprintf(buf, sizeof(buf), "%d linh thach", st.spiritStones);
  drawTextRight(WIDTH - margin, 226, buf, 2, false);

  drawProgressBar(margin, 266, innerW, 14, st.realmProgressPct);

  snprintf(buf, sizeof(buf), "Tu vi %d%% . tien toi %s",
           st.realmProgressPct, CultivationManager::getNextRealmName(st.realmIndex).c_str());
  drawText(margin, 290, buf, 1, true);

  // ================= 3. PHÁP BẢO BẢN MỆNH =================
  int artIdx = (st.boundArtifactId >= 0 && st.boundArtifactId < CultivationManager::TOTAL_ARTIFACTS)
                 ? st.boundArtifactId : 0;
  const auto& art = CultivationManager::ARTIFACTS[artIdx];
  int artY = 328;

  drawRect(margin, artY, 84, 84, true);
  drawTextCentered(margin + 42, artY + 34, art.grade, 2, true);

  drawText(margin + 100, artY + 2, "BAN MENH", 1, false);
  drawText(margin + 100, artY + 22, art.name, 2, true);
  drawText(margin + 100, artY + 58, art.buff, 1, false);

  snprintf(buf, sizeof(buf), "%d/36 phap bao", CultivationManager::getUnlockedArtifactCount(st));
  drawTextRight(WIDTH - margin, artY + 58, buf, 1, false);

  // ================= 4. CÔNG PHÁP & LÒ LUYỆN =================
  int bookY = 436;
  drawText(margin, bookY, st.currentBookTitle.c_str(), 2, true);

  int skillLayer = (st.currentBookPct * 9) / 100;
  if (skillLayer < 1 && st.currentBookPct > 0) skillLayer = 1;
  snprintf(buf, sizeof(buf), "Cong phap tang %d/9 . %d%%", skillLayer, st.currentBookPct);
  drawText(margin, bookY + 32, buf, 1, true);

  drawProgressBar(margin, bookY + 56, innerW, 10, st.currentBookPct);

  snprintf(buf, sizeof(buf), "Lo luyen: %s",
           st.furnaceBookName.length() > 0 ? st.furnaceBookName.c_str() : "trong");
  drawText(margin, bookY + 82, buf, 1, false);

  // ================= 5. MỤC TIÊU NGÀY & LỊCH TUẦN =================
  int dailyY = 572;
  snprintf(buf, sizeof(buf), "Hom nay %d/%d phut", st.todayMinutes, st.targetMinutes);
  drawText(margin, dailyY, buf, 2, false);

  int dailyPct = (st.targetMinutes > 0) ? ((st.todayMinutes * 100) / st.targetMinutes) : 0;
  if (dailyPct > 100) dailyPct = 100;
  drawProgressBar(margin + 260, dailyY + 8, innerW - 260, 10, dailyPct);

  const char* dayLabels[] = {"T2", "T3", "T4", "T5", "T6", "T7", "CN"};
  const int boxW = 58;
  const int gap = (innerW - boxW * 7) / 6;
  const int boxY = 618;

  for (int i = 0; i < 7; i++) {
    int bx = margin + i * (boxW + gap);
    drawRect(bx, boxY, boxW, 30, true);
    if (st.weekCompleted[i]) {
      fillRect(bx + 3, boxY + 3, boxW - 6, 24, true);
    }
    drawTextCentered(bx + boxW / 2, boxY + 38, dayLabels[i], 2, false);
  }

  // ================= 6. CHÂN TRANG =================
  fillRect(margin, 718, innerW, 2, true);

  snprintf(buf, sizeof(buf), "Chuoi %d ngay . Bua %d", st.streakDays, st.protectionTalismans);
  drawText(margin, 738, buf, 2, false);

  snprintf(buf, sizeof(buf), "Tuan %d/5 ngay", st.weeklyDoneDays);
  drawTextRight(WIDTH - margin, 738, buf, 2, false);
}
