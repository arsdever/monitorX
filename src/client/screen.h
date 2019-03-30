#ifndef SCREEN_H
#define SCREEN_H

#include "abstract_screen.h"

class Screen : public AbstractScreen
{
public:
  Screen(LiquidCrystal *lcd, uint8_t width, uint8_t height);
  ~Screen() override {}

  void scrollDown() override;
  void scrollUp() override;
  void draw() const override;

  void setMenuCount(uint8_t menu_count);

  uint8_t visibleAreaPosition() const;
  uint8_t getScrollerPosition() const;

private:
  static uint8_t fill[8];
  static uint8_t arrow_up[8];
  static uint8_t arrow_down[8];

  uint8_t __menu_count;
  uint8_t __visible_area_position;
  uint8_t __scroller_position;
};

#endif