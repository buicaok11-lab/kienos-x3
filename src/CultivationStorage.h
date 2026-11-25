#pragma once
#include "CultivationManager.h"

class CultivationStorage {
public:
  static constexpr const char* SAVE_PATH = "/kienos_tutien.dat";

  static bool beginSD(int csPin = 5);
  static bool saveState(const CultivationState& st);
  static bool loadState(CultivationState& st);
};
