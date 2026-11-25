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

  if (CultivationStorage::beginSD()) {
    CultivationStorage::loadState(g_state);
  }

  LunarDate ld = LunarCalendar::solarToLunar(25, 11, 2026);
  CultivationManager::recordReadingSession(
      g_state, 15, 1200, true, false, ld.isDoubleSpiritStoneDay());

  g_sleepScreen.render(g_state, ld, 2026, 11, 25, 3);
  CultivationStorage::saveState(g_state);
}

void loop() {
  delay(1000);
}
