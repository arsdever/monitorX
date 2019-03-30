#include "screen.h"

#include <LiquidCrystal.h>

uint8_t Screen::arrow_up[8] = {0, 4, 4, 14, 14, 31, 31, 0};
uint8_t Screen::arrow_down[8] = {0, 31, 31, 14, 14, 4, 4, 0};
uint8_t Screen::fill[8] = {31, 31, 31, 31, 31, 31, 31, 31};

Screen::Screen(LiquidCrystal *lcd, uint8_t width, uint8_t height)
    : AbstractScreen(lcd, width, height)
{
}

void Screen::setMenuCount(uint8_t menu_count)
{
    __menu_count = menu_count;
}

uint8_t Screen::visibleAreaPosition() const
{
    return __visible_area_position;
}

uint8_t Screen::getScrollerPosition() const
{
    return __scroller_position;
}

void Screen::scrollUp()
{
    if (__scroller_position == 1)
    {
        __scroller_position = __menu_count;
        int diff = __menu_count - height();
        if (diff <= 0)
            __visible_area_position = 1;

        __visible_area_position = diff;
    }

    if (__visible_area_position > --__scroller_position)
        __visible_area_position = __scroller_position;
}

void Screen::scrollDown()
{
    if (__scroller_position == __menu_count)
    {
        __scroller_position = 1;
        __visible_area_position = 1;
    }

    if (__visible_area_position < __scroller_position - __menu_count)
    {
        __visible_area_position = __scroller_position - __menu_count;
    }
}

void Screen::draw() const
{
    device()->createChar(5, fill);
    device()->createChar(6, arrow_up);
    device()->createChar(7, arrow_down);
    device()->setCursor(width() - 1, 0);
    device()->print((char)6);
    device()->setCursor(width() - 1, height() - 1);
    device()->print((char)7);

    if (height() <= 2)
        return;

    uint8_t position = (__scroller_position * (height() - 2)) / __menu_count;

    device()->setCursor(width() - 1, position + 1);
    device()->print((char)5);
}