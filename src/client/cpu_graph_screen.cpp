#include "cpu_graph_screen.h"

#include "data.h"
#include <LiquidCrystal.h>

uint8_t CPUGraphScreen::__1[8] = {0, 0, 0, 0, 0, 0, 0, 31};
uint8_t CPUGraphScreen::__2[8] = {0, 0, 0, 0, 0, 0, 31, 31};
uint8_t CPUGraphScreen::__3[8] = {0, 0, 0, 0, 0, 31, 31, 31};
uint8_t CPUGraphScreen::__4[8] = {0, 0, 0, 0, 31, 31, 31, 31};
uint8_t CPUGraphScreen::__5[8] = {0, 0, 0, 31, 31, 31, 31, 31};
uint8_t CPUGraphScreen::__6[8] = {0, 0, 31, 31, 31, 31, 31, 31};
uint8_t CPUGraphScreen::__7[8] = {0, 31, 31, 31, 31, 31, 31, 31};
uint8_t CPUGraphScreen::__8[8] = {31, 31, 31, 31, 31, 31, 31, 31};

CPUGraphScreen::CPUGraphScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data)
    : InfoScreen(lcd, width, height), __history(new uint8_t[width]{0})
{
    InfoScreen::setDataSource(data);
    setMenuCount(3);
}

CPUGraphScreen::~CPUGraphScreen() {}

void CPUGraphScreen::draw() const
{
    for (uint8_t i = 0; i < width() - 1; ++i)
    {
        __history[i] = __history[i + 1];
    }

    const INFO *data = getDataSource();
    __history[width() - 1] = data->cpuUsageKernel + data->cpuUsageUser;

    device()->createChar(0, __1);
    device()->createChar(1, __2);
    device()->createChar(2, __3);
    device()->createChar(3, __4);
    device()->createChar(4, __5);
    device()->createChar(5, __6);
    device()->createChar(6, __7);
    device()->createChar(7, __8);

    for (uint8_t i = 0; i < width(); ++i)
    {
        float percent = __history[i];
        float min_percent = 100.0 / height();
        for (uint8_t j = 0; j < height(); ++j)
        {
            device()->setCursor(i, height() - j - 1);
            if (percent > min_percent)
            {
                device()->print((char)7);
            }
            else
            {
                char ch = percent * 8 / min_percent;
                if (ch == 0)
                {
                    device()->print(" ");
                }
                else
                {
                    device()->print((char)(ch - 1));
                }
            }
            percent -= min_percent;
            if (percent < 0)
                break;
        }
    }
}