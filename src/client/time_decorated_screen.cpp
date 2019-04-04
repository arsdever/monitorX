#include "time_decorated_screen.h"

#include "info_screen.h"
#include "data.h"
#include <LiquidCrystal.h>
#include <Arduino.h>

TimeDecoratedScreen::TimeDecoratedScreen(InfoScreen *screen)
    : AbstractScreenDecorator(screen), __show_seconds(false)
{
}

void TimeDecoratedScreen::draw() const
{
    AbstractScreenDecorator::draw();
    String printing_msg = timeToString();
    device()->setCursor(width() - printing_msg.length(), 0);
    device()->print(printing_msg);
}

void TimeDecoratedScreen::showSeconds(bool b)
{
    if (b)
        __show_seconds = true;
    else
        __show_seconds = false;
}

void TimeDecoratedScreen::hideSeconds(bool b)
{
    showSeconds(!b);
}

String TimeDecoratedScreen::timeToString() const
{
    const INFO *data = ((InfoScreen *)getBaseScreen())->getDataSource();
    String result = normalize(data->hour) + ':' + normalize(data->minu);
    if (__show_seconds)
        result += normalize(data->sec);

    return result;
}