#ifndef RAM_SCREEN_H
#define RAM_SCREEN_H

#include "info_screen.h"

class RAMScreen : public InfoScreen
{
  public:
    RAMScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data = nullptr);
    ~RAMScreen() override;

    void draw() const override;
};

#endif