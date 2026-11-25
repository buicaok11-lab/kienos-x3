#include "CultivationStorage.h"
#include <SD.h>
#include <SPI.h>

bool CultivationStorage::beginSD(int csPin) {
  return SD.begin(csPin);
}

bool CultivationStorage::saveState(const CultivationState& st) {
  File f = SD.open(SAVE_PATH, FILE_WRITE);
  if (!f) return false;

  // Ghi phần cố định (POD fields)
  f.write((const uint8_t*)&st.realmIndex, sizeof(st.realmIndex));
  f.write((const uint8_t*)&st.realmLayer, sizeof(st.realmLayer));
  f.write((const uint8_t*)&st.realmProgressPct, sizeof(st.realmProgressPct));
  f.write((const uint8_t*)&st.spiritStones, sizeof(st.spiritStones));
  f.write((const uint8_t*)&st.oreCount, sizeof(st.oreCount));
  f.write((const uint8_t*)&st.boundArtifactId, sizeof(st.boundArtifactId));
  f.write(st.ownedArtifacts, sizeof(st.ownedArtifacts));
  f.write((const uint8_t*)&st.duplicateCount, sizeof(st.duplicateCount));
  f.write((const uint8_t*)&st.currentBookPct, sizeof(st.currentBookPct));
  f.write((const uint8_t*)&st.furnacePagesDone, sizeof(st.furnacePagesDone));
  f.write((const uint8_t*)&st.furnacePagesTarget, sizeof(st.furnacePagesTarget));
  f.write((const uint8_t*)&st.todayMinutes, sizeof(st.todayMinutes));
  f.write((const uint8_t*)&st.targetMinutes, sizeof(st.targetMinutes));
  f.write((const uint8_t*)&st.streakDays, sizeof(st.streakDays));
  f.write((const uint8_t*)&st.protectionTalismans, sizeof(st.protectionTalismans));
  f.write((const uint8_t*)&st.weeklyDoneDays, sizeof(st.weeklyDoneDays));
  f.write((const uint8_t*)st.weekCompleted, sizeof(st.weekCompleted));

  // Ghi chuỗi tên sách đang đọc & sách trong lò luyện
  uint16_t titleLen = (uint16_t)st.currentBookTitle.length();
  f.write((const uint8_t*)&titleLen, sizeof(titleLen));
  if (titleLen > 0) {
    f.write((const uint8_t*)st.currentBookTitle.c_str(), titleLen);
  }

  uint16_t furnaceLen = (uint16_t)st.furnaceBookName.length();
  f.write((const uint8_t*)&furnaceLen, sizeof(furnaceLen));
  if (furnaceLen > 0) {
    f.write((const uint8_t*)st.furnaceBookName.c_str(), furnaceLen);
  }

  f.close();
  return true;
}

bool CultivationStorage::loadState(CultivationState& st) {
  if (!SD.exists(SAVE_PATH)) return false;
  File f = SD.open(SAVE_PATH, FILE_READ);
  if (!f) return false;

  f.read((uint8_t*)&st.realmIndex, sizeof(st.realmIndex));
  f.read((uint8_t*)&st.realmLayer, sizeof(st.realmLayer));
  f.read((uint8_t*)&st.realmProgressPct, sizeof(st.realmProgressPct));
  f.read((uint8_t*)&st.spiritStones, sizeof(st.spiritStones));
  f.read((uint8_t*)&st.oreCount, sizeof(st.oreCount));
  f.read((uint8_t*)&st.boundArtifactId, sizeof(st.boundArtifactId));
  f.read(st.ownedArtifacts, sizeof(st.ownedArtifacts));
  f.read((uint8_t*)&st.duplicateCount, sizeof(st.duplicateCount));
  f.read((uint8_t*)&st.currentBookPct, sizeof(st.currentBookPct));
  f.read((uint8_t*)&st.furnacePagesDone, sizeof(st.furnacePagesDone));
  f.read((uint8_t*)&st.furnacePagesTarget, sizeof(st.furnacePagesTarget));
  f.read((uint8_t*)&st.todayMinutes, sizeof(st.todayMinutes));
  f.read((uint8_t*)&st.targetMinutes, sizeof(st.targetMinutes));
  f.read((uint8_t*)&st.streakDays, sizeof(st.streakDays));
  f.read((uint8_t*)&st.protectionTalismans, sizeof(st.protectionTalismans));
  f.read((uint8_t*)&st.weeklyDoneDays, sizeof(st.weeklyDoneDays));
  f.read((uint8_t*)st.weekCompleted, sizeof(st.weekCompleted));

  uint16_t titleLen = 0;
  if (f.read((uint8_t*)&titleLen, sizeof(titleLen)) == sizeof(titleLen) && titleLen > 0 && titleLen < 256) {
    char buf[256];
    f.read((uint8_t*)buf, titleLen);
    buf[titleLen] = '\0';
    st.currentBookTitle = String(buf);
  }

  uint16_t furnaceLen = 0;
  if (f.read((uint8_t*)&furnaceLen, sizeof(furnaceLen)) == sizeof(furnaceLen) && furnaceLen > 0 && furnaceLen < 256) {
    char buf[256];
    f.read((uint8_t*)buf, furnaceLen);
    buf[furnaceLen] = '\0';
    st.furnaceBookName = String(buf);
  }

  f.close();
  return true;
}
