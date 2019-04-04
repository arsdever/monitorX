#include "date_decorated_screen.h"

#include "info_screen.h"
#include "data.h"
#include <LiquidCrystal.h>

DateDecoratedScreen::DateDecoratedScreen(InfoScreen *screen)
    : AbstractScreenDecorator(screen)
{
}

void DateDecoratedScreen::draw() const
{
    AbstractScreenDecorator::draw();
    String msg = dateToString();
    device()->setCursor(width() - msg.length(), 0);
    device()->print(msg);
}

String DateDecoratedScreen::dateToString() const
{
    InfoScreen *screen = static_cast<InfoScreen *>(getBaseScreen());
    if (screen == nullptr)
        return String();

    return normalize(screen->getDataSource()->day) + '.' + normalize(screen->getDataSource()->month) + '.' + normalizeYear(screen->getDataSource()->year);
}