#include "CultivationManager.h"

const ArtifactInfo CultivationManager::ARTIFACTS[CultivationManager::TOTAL_ARTIFACTS] = {
  {0,  "Thanh Đồng Chung",   "Phàm", "Tụ Linh: +20% linh thạch"},
  {1,  "Huyền Thiết Kiếm",   "Phàm", "Kiếm Ý: +15% tu vi đọc chữ"},
  {2,  "Ngọc Tiêu Bích",     "Phàm", "Tĩnh Tâm: +10% tốc độ luyện"},
  {3,  "Hỏa Lân Đỉnh",       "Phàm", "Luyện Khí: lò luyện +15%"},
  {4,  "Vân Tiêu Phiến",     "Phàm", "Thanh Phong: đọc đêm x2 tu vi"},
  {5,  "Hắc Ma Phiên",       "Phàm", "Nhiếp Hồn: tăng cơ hội kỳ ngộ"},
  {6,  "Linh Mộc Trượng",    "Phàm", "Sinh Cơ: dễ nhận bùa hộ pháp"},
  {7,  "Thạch Giáp Thuẫn",   "Phàm", "Hộ Thân: giữ chuỗi ngày đọc"},
  {8,  "Kim Quang La Bàn",   "Linh", "Tầm Bảo: +25% linh thạch"},
  {9,  "U Minh Đao",         "Linh", "Trảm Vọng: +20% tu vi"},
  {10, "Băng Phách Châu",    "Linh", "Hàn Khí: đọc đêm x2 linh thạch"},
  {11, "Lôi Đình Chùy",      "Linh", "Dẫn Lôi: vượt thiên kiếp nhanh"},
  {12, "Cửu Khúc Cầm",       "Linh", "Thần Âm: +20% tu vi công pháp"},
  {13, "Xích Diễm Kiếm",     "Linh", "Hỏa Diễm: lò luyện +20%"},
  {14, "Thanh Liên Đăng",    "Linh", "Hộ Đạo: +1 bùa khi đủ tuần"},
  {15, "Huyền Vũ Ấn",        "Linh", "Trấn Thủ: +25% linh thạch"},
  {16, "Thái Ất Phất Trần", "Bảo",  "Pháp Lực: +30% linh thạch"},
  {17, "Vạn Tượng Kính",     "Bảo",  "Ngộ Đạo: +30% tu vi"},
  {18, "Tử Tiêu Hồ Lô",     "Bảo",  "Hấp Linh: đọc đêm x2 linh thạch"},
  {19, "Bát Quái Đồ",        "Bảo",  "Diễn Toán: lò luyện +30%"},
  {20, "Tru Tiên Trận Đồ",   "Bảo",  "Sát Khí: +35% tu vi công pháp"},
  {21, "Hỗn Nguyên Châu",    "Bảo",  "Hỗn Độn: +35% linh thạch"},
  {22, "Phượng Vũ Kỳ",       "Bảo",  "Niết Bàn: giữ chuỗi ngày đọc"},
  {23, "Long Ngâm Cổ Kiếm",  "Bảo",  "Long Uy: +35% tu vi"},
  {24, "Càn Khôn Đỉnh",      "Bảo",  "Luyện Hóa: lò luyện +35%"},
  {25, "Tinh Thần Tháp",     "Bảo",  "Tinh Hoa: đọc đêm x2 tu vi"},
  {26, "Hư Không Tráo",      "Bảo",  "Xuyên Không: +40% linh thạch"},
  {27, "Vô Cực Ấn",          "Bảo",  "Vô Cực: +40% tu vi"},
  {28, "Tạo Hóa Ngọc Điệp",  "Tiên", "Tạo Hóa: +50% linh thạch & tu vi"},
  {29, "Khai Thiên Phủ",      "Tiên", "Khai Thiên: lò luyện +50%"},
  {30, "Thái Sơ Kiếm Thai",  "Tiên", "Tiên Kiếm: +50% tu vi công pháp"},
  {31, "Hồng Mông Châu",     "Tiên", "Hồng Mông: đọc đêm x2 mọi thưởng"},
  {32, "Định Hải Thần Châm", "Tiên", "Định Hải: +50% linh thạch"},
  {33, "Luân Hồi Bàn",       "Tiên", "Luân Hồi: không bao giờ mất chuỗi"},
  {34, "Cửu Thiên Huyền Nữ", "Tiên", "Tiên Duyên: kỳ ngộ nhân đôi"},
  {35, "Bổ Thiên Thạch",     "Tiên", "Bổ Thiên: viên mãn cảnh giới"}
};

String CultivationManager::getRealmName(int idx) {
  switch (idx) {
    case 0: return "Luyện Khí";
    case 1: return "Trúc Cơ";
    case 2: return "Kim Đan";
    case 3: return "Nguyên Anh";
    default: return "Tiên Nhân";
  }
}

String CultivationManager::getNextRealmName(int idx) {
  if (idx >= 4) return "Viên Mãn";
  return getRealmName(idx + 1);
}

int CultivationManager::getUnlockedArtifactCount(const CultivationState& st) {
  int count = 0;
  for (int i = 0; i < TOTAL_ARTIFACTS; i++) {
    int byteIdx = i / 8;
    int bitIdx = i % 8;
    if (st.ownedArtifacts[byteIdx] & (1 << bitIdx)) {
      count++;
    }
  }
  return count;
}

void CultivationManager::recordReadingSession(
    CultivationState& st,
    int minutes,
    int wordsRead,
    bool finishedChapter,
    bool isNightTime,
    bool isDoubleSpiritDay) {
  if (minutes < 0) minutes = 0;
  if (wordsRead < 0) wordsRead = 0;

  st.todayMinutes += minutes;

  // Hệ số từ pháp bảo bản mệnh
  float stoneMult = 1.0f;
  float expMult = 1.0f;
  if (st.boundArtifactId == 0) stoneMult += 0.20f; // Thanh Đồng Chung
  if (st.boundArtifactId == 1) expMult += 0.15f;   // Huyền Thiết Kiếm
  if (isNightTime && (st.boundArtifactId == 4 || st.boundArtifactId == 25 || st.boundArtifactId == 31)) {
    expMult *= 2.0f;
  }
  if (isNightTime && (st.boundArtifactId == 10 || st.boundArtifactId == 18 || st.boundArtifactId == 31)) {
    stoneMult *= 2.0f;
  }
  if (isDoubleSpiritDay) {
    stoneMult *= 2.0f; // Ngày rằm & mùng 1 linh thạch nhân đôi
  }

  // Cộng tu vi theo số chữ đã đọc
  int addedExpPct = (int)((wordsRead / 250.0f) * expMult);
  if (addedExpPct < 1 && wordsRead > 0) addedExpPct = 1;
  st.realmProgressPct += addedExpPct;

  while (st.realmProgressPct >= 100) {
    st.realmProgressPct -= 100;
    st.realmLayer++;
    if (st.realmLayer > 9) {
      st.realmLayer = 1;
      if (st.realmIndex < 4) st.realmIndex++;
    }
  }

  // Hết chương có kỳ ngộ -> nhặt linh thạch
  if (finishedChapter) {
    int baseStone = 15;
    st.spiritStones += (int)(baseStone * stoneMult);
  }

  // Lò luyện: tích lũy trang đọc để luyện pháp bảo
  if (!st.furnaceBookName.isEmpty() && minutes > 0) {
    int pagesGain = minutes * 3;
    st.furnacePagesDone += pagesGain;
    if (st.furnacePagesDone >= st.furnacePagesTarget) {
      st.furnacePagesDone = 0;
      int nextId = (getUnlockedArtifactCount(st)) % TOTAL_ARTIFACTS;
      int byteIdx = nextId / 8;
      int bitIdx = nextId % 8;
      if (st.ownedArtifacts[byteIdx] & (1 << bitIdx)) {
        st.duplicateCount++; // Pháp bảo trùng
      } else {
        st.ownedArtifacts[byteIdx] |= (1 << bitIdx);
      }
      st.furnaceBookName = "";
    }
  }
}

bool CultivationManager::smeltDuplicateArtifacts(CultivationState& st) {
  if (st.duplicateCount < 3) return false;
  st.duplicateCount -= 3;
  st.oreCount += 1;
  return true;
}
