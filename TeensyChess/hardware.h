// SPDX-License-Identifier: GPL-3.0-or-later
// Teensy Chess - the display and touch controller objects (defined in TeensyChess.ino)

#pragma once

#include <ST7796_t3.h>
//#include <Adafruit_FT6206.h>
#include <XPT2046_Touchscreen.h>

extern ST7796_t3 tft;
#if __has_include(<Adafruit_FT6206.h>)
extern Adafruit_FT6206 touch;
#elif __has_include(<XPT2046_Touchscreen.h>)
extern XPT2046_Touchscreen touch;
#endif
