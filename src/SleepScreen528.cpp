#include "SleepScreen528.h"
#include <cstring>
#include <cstdio>

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

void SleepScreen528::render(const CultivationState& st, const LunarDate& lunar,
                           int year, int month, int day, int weekdayIdx) {
  clear(true);

  const int margin = 28;
  const int innerW = WIDTH - margin * 2; // 472

  // 1. Đường phân cách Lịch Việt & Khu vực Tu Tiên
  fillRect(margin, 204, innerW, 3, true);

  // 2. Thanh tiến độ Tu Vi (dưới dòng Cảnh giới · Tầng X)
  drawProgressBar(margin, 268, innerW, 14, st.realmProgressPct);

  // 3. Khung vuông Pháp bảo bản mệnh (82x82)
  drawRect(margin, 332, 82, 82, true);

  // 4. Thanh tiến độ Công pháp (sách đang đọc)
  drawProgressBar(margin, 504, innerW, 10, st.currentBookPct);

  // 5. Thanh tiến độ Mục tiêu đọc hôm nay
  int dailyPct = (st.targetMinutes > 0) ? ((st.todayMinutes * 100) / st.targetMinutes) : 0;
  if (dailyPct > 100) dailyPct = 100;
  drawProgressBar(margin + 230, 582, innerW - 230, 10, dailyPct);

  // 6. 7 ô ngày trong tuần (T2..CN)
  const int boxW = 58;
  const int gap = (innerW - boxW * 7) / 6;
  const int boxY = 622;
  for (int i = 0; i < 7; i++) {
    int bx = margin + i * (boxW + gap);
    drawRect(bx, boxY, boxW, 30, true);
    if (i < 7 && st.weekCompleted[i]) {
      fillRect(bx + 3, boxY + 3, boxW - 6, 24, true);
    }
  }

  // 7. Đường phân cách chân trang
  fillRect(margin, 718, innerW, 2, true);

  // Log tóm tắt trạng thái màn hình Sleep ra Serial để kiểm tra
  const char* weekdays[] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};
  const char* wd = (weekdayIdx >= 0 && weekdayIdx < 7) ? weekdays[weekdayIdx] : "T3";
  int artIdx = (st.boundArtifactId >= 0 && st.boundArtifactId < CultivationManager::TOTAL_ARTIFACTS)
                 ? st.boundArtifactId : 0;
  const auto& art = CultivationManager::ARTIFACTS[artIdx];

  Serial.printf("[Sleep528] %02d/%02d/%04d (%s) | Am: %02d/%02d (%s)\n",
                day, month, year, wd, lunar.day, lunar.month, lunar.dayCanChi.c_str());
  Serial.printf("[Sleep528] %s Tang %d (%d%%) -> %s | %d linh thach\n",
                CultivationManager::getRealmName(st.realmIndex).c_str(),
                st.realmLayer, st.realmProgressPct,
                CultivationManager::getNextRealmName(st.realmIndex).c_str(),
                st.spiritStones);
  Serial.printf("[Sleep528] Ban menh: [%s] %s (%s) | %d/36 phap bao\n",
                art.grade, art.name, art.buff,
                CultivationManager::getUnlockedArtifactCount(st));
  Serial.printf("[Sleep528] Hom nay: %d/%d phut | Chuoi %d ngay | Bua %d | Tuan %d/5\n",
                st.todayMinutes, st.targetMinutes, st.streakDays,
                st.protectionTalismans, st.weeklyDoneDays);
}
