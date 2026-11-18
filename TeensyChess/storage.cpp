// SPDX-License-Identifier: GPL-3.0-or-later
// Teensy Chess - settings saved in the Teensy's EEPROM

#include "storage.h"

#include <EEPROM.h>

#include "config.h"

namespace {

const uint32_t SETTINGS_MAGIC = 0x54434831;  // "TCH1"
const int      SETTINGS_ADDR  = 0;

struct StoredSettings {
  uint32_t magic;
  Settings data;
};

}  // namespace

Settings settings;

bool calibrationValid() {
  const Settings& s = settings;
  if (abs(s.calLeft) < 0 || abs(s.calLeft) > 479 || abs(s.calRight) < 0 || abs(s.calRight) > 479) return false;
  if (abs(s.calTop) < 0 || abs(s.calTop) > 340 || abs(s.calBottom) < 0 || abs(s.calBottom) > 340) return false;
  return abs(s.calRight - s.calLeft) > 100  && abs(s.calBottom - s.calTop) > 100;
}

bool loadSettings() {
  StoredSettings stored;
  EEPROM.get(SETTINGS_ADDR, stored);
  if (stored.magic != SETTINGS_MAGIC) {
    Serial.println("No saved settings, using defaults");
    return false;
  }

  Settings old = settings;
  settings = stored.data;
  if (settings.level > MAX_LEVEL) settings.level = DEFAULT_LEVEL;
  settings.humanWhite = settings.humanWhite ? 1 : 0;

  if (!calibrationValid()) {
    Serial.println("Saved touch calibration is invalid");
    settings.calLeft = old.calLeft;
    settings.calRight = old.calRight;
    settings.calTop = old.calTop;
    settings.calBottom = old.calBottom;
    return false;
  }

  Serial.printf("Settings loaded: touch L %d R %d T %d B %d, level %d, player %s\n", settings.calLeft,
                settings.calRight, settings.calTop, settings.calBottom, settings.level,
                settings.humanWhite ? "white" : "black");
  return true;
}

void saveSettings() {
  StoredSettings stored;
  stored.magic = SETTINGS_MAGIC;
  stored.data = settings;
  EEPROM.put(SETTINGS_ADDR, stored);  // put() only rewrites bytes that differ
}
