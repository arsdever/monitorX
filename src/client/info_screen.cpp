#include "info_screen.h"

InfoScreen::InfoScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height)
    : Screen(lcd, width, height)
{
}

void InfoScreen::setDataSource(INFO *data)
{
    __data = data;
}

const INFO *InfoScreen::getDataSource() const
{
    return __data;
}