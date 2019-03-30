#ifndef HDD_SCREEN_H
#define HDD_SCREEN_H

#include "info_screen.h"

class HDDScreen : public InfoScreen
{
  public:
    HDDScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data = nullptr);
    ~HDDScreen() override;

    void draw() const override;
};

#endif