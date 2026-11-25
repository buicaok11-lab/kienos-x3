#include <Arduino.h>
#include "LunarCalendar.h"
#include "CultivationManager.h"

CultivationState g_state;

void setup() {
  Serial.begin(115200);
  delay(200);

  LunarDate ld = LunarCalendar::solarToLunar(25, 11, 2026);
  CultivationManager::recordReadingSession(g_state, 15, 1200, true, false, ld.isDoubleSpiritDay());

  Serial.printf("Canh gioi: %s Tang %d | Linh thach: %d | Phap bao: %d/36\n",
                CultivationManager::getRealmName(g_state.realmIndex).c_str(),
                g_state.realmLayer,
                g_state.spiritStones,
                CultivationManager::getUnlockedArtifactCount(g_state));
}

void loop() {
  delay(1000);
}
