#include "cpu_screen.h"

#include "data.h"
#include <LiquidCrystal.h>

CPUScreen::CPUScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data)
    : InfoScreen(lcd, width, height)
{
    InfoScreen::setDataSource(data);
    setMenuCount(3);
}

CPUScreen::~CPUScreen() {}

void CPUScreen::draw() const
{
    INFO *data = getDataSource();
    device()->setCursor(0, 0);
    device()->print("CPU Info");
    device()->setCursor(0, 1);
    device()->print(String("  cores : ") + String((uint8_t)data->cores));
    device()->setCursor(0, 2);
    device()->print(String("  kernel: ") + String((uint8_t)data->cpuUsageKernel) + String("%"));
    device()->setCursor(0, 3);
    device()->print(String("  user  : ") + String((uint8_t)data->cpuUsageUser) + String("%"));
}