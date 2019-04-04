#include "abstract_screen_decorator.h"

#include <LiquidCrystal.h>
#include <Arduino.h>

AbstractScreenDecorator::AbstractScreenDecorator(AbstractScreen *screen)
    : AbstractScreen(nullptr, 0, 0), __screen(screen)
{
}

LiquidCrystal *AbstractScreenDecorator::device() const
{
    return __screen->device();
}

void AbstractScreenDecorator::draw() const
{
    __screen->draw();
}

void AbstractScreenDecorator::scrollDown()
{
    __screen->scrollDown();
}

void AbstractScreenDecorator::scrollUp()
{
    __screen->scrollUp();
}

uint8_t AbstractScreenDecorator::width() const
{
    return __screen->width();
}

uint8_t AbstractScreenDecorator::height() const
{
    return __screen->height();
}

AbstractScreen *AbstractScreenDecorator::getBaseScreen() const
{
    return __screen;
}