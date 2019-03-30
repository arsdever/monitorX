#ifndef CPU_GRAPH_SCREEN_H
#define CPU_GRAPH_SCREEN_H

#include "info_screen.h"

class CPUGraphScreen : public InfoScreen
{
public:
  CPUGraphScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data = nullptr);
  ~CPUGraphScreen() override;

  void draw() const override;

private:
  static uint8_t __1[8];
  static uint8_t __2[8];
  static uint8_t __3[8];
  static uint8_t __4[8];
  static uint8_t __5[8];
  static uint8_t __6[8];
  static uint8_t __7[8];
  static uint8_t __8[8];

  uint8_t *__history;
};

#endif