#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include "screen.h"

struct INFO;

class InfoScreen : public Screen
{
  public:
    InfoScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height);

    void setDataSource(INFO *data);
    const INFO *getDataSource() const;

  private:
    INFO *__data;
};

#endif