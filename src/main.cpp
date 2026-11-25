#include <Arduino.h>
#include "LunarCalendar.h"
#include "CultivationManager.h"
#include "CultivationStorage.h"
#include "SleepScreen528.h"

CultivationState g_state;
SleepScreen528 g_sleepScreen;

void setup() {
  Serial.begin(115200);
  delay(200);

  bool sdReady = CultivationStorage::beginSD();
  if (sdReady) {
    CultivationStorage::loadState(g_state);
  }

  // Thiết lập mặc định ban đầu giống giao diện mẫu nếu chưa có dữ liệu
  if (g_state.currentBookTitle.length() == 0) {
    g_state.currentBookTitle = "Pham Nhan Tu Tien Truyen - Vong Ngu";
  }

  // Ví dụ mô phỏng phiên đọc: 35 phút, 1500 chữ, có kỳ ngộ hết chương
  LunarDate ld = LunarCalendar::solarToLunar(25, 11, 2026);
  CultivationManager::recordReadingSession(
      g_state, 5, 600, true, false, ld.isDoubleSpiritStoneDay());

  // Đổi 3 pháp bảo trùng -> 1 khoáng thạch nếu đủ
  CultivationManager::smeltDuplicateArtifacts(g_state);

  // Vẽ màn hình Sleep 528x792
  g_sleepScreen.render(g_state, ld, 2026, 11, 25, 3);

  if (sdReady) {
    CultivationStorage::saveState(g_state);
    g_sleepScreen.saveToBMP("/sleep_tutien.bmp");
  }
}

void loop() {
  delay(1000);
}
