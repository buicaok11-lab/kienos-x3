#pragma once
#include <Arduino.h>

struct ArtifactInfo {
  int id;
  const char* name;
  const char* grade;   // "Phàm", "Linh", "Bảo", "Tiên"
  const char* buff;    // Thiên phú riêng
};

struct CultivationState {
  // Cảnh giới & Tu vi
  int realmIndex = 1;          // 0: Luyện Khí, 1: Trúc Cơ, 2: Kim Đan, 3: Nguyên Anh, 4: Tiên Nhân
  int realmLayer = 1;          // Tầng nhỏ (1..9)
  int realmProgressPct = 5;    // 0..100%

  // Tài nguyên & Pháp bảo
  int spiritStones = 439;      // Linh thạch
  int oreCount = 0;            // Khoáng thạch (đổi từ 3 pháp bảo trùng)
  int boundArtifactId = 0;     // Pháp bảo bản mệnh (0..35)
  uint8_t ownedArtifacts[5] = {0b00000001, 0, 0, 0, 0}; // Bitmask 36 pháp bảo
  int duplicateCount = 0;      // Số pháp bảo trùng đang giữ

  // Công pháp (sách đang đọc) & Lò luyện
  String currentBookTitle = "Phàm Nhân Tu Tiên Truyện - Vong Ngữ";
  int currentBookPct = 37;     // 0..100% -> tầng 1..9
  String furnaceBookName = ""; // Rỗng = trống
  int furnacePagesDone = 0;
  int furnacePagesTarget = 120;

  // Thời gian đọc, chuỗi ngày, bùa hộ pháp & mốc tuần
  int todayMinutes = 35;
  int targetMinutes = 30;
  int streakDays = 12;
  int protectionTalismans = 0; // Bùa hộ pháp
  int weeklyDoneDays = 2;      // Mục tiêu 5 ngày/tuần
  bool weekCompleted[7] = {true, true, false, false, false, false, false}; // T2..CN
};

class CultivationManager {
public:
  static constexpr int TOTAL_ARTIFACTS = 36;
  static const ArtifactInfo ARTIFACTS[TOTAL_ARTIFACTS];

  static String getRealmName(int idx);
  static String getNextRealmName(int idx);
  static int getUnlockedArtifactCount(const CultivationState& st);

  // Ghi nhận phiên đọc (phút, số chữ, hết chương hay không, có phải ban đêm 22h-4h, có phải Rằm/Mùng 1)
  static void recordReadingSession(
      CultivationState& st,
      int minutes,
      int wordsRead,
      bool finishedChapter,
      bool isNightTime,
      bool isDoubleSpiritDay);

  // Đổi 3 pháp bảo trùng -> 1 khoáng thạch hạng cao hơn
  static bool smeltDuplicateArtifacts(CultivationState& st);
};
