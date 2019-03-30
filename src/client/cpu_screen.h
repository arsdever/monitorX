#ifndef CPU_SCREEN_H
#define CPU_SCREEN_H

#include "info_screen.h"

class CPUScreen : public InfoScreen
{
public:
  CPUScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data = nullptr);
  ~CPUScreen() override;

  void draw() const override;
};

#endif