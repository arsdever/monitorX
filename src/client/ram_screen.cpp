#include "ram_screen.h"

#include "data.h"
#include <LiquidCrystal.h>

RAMScreen::RAMScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height, INFO *data)
    : InfoScreen(lcd, width, height)
{
    InfoScreen::setDataSource(data);
    setMenuCount(3);
}

RAMScreen::~RAMScreen() {}

void RAMScreen::draw() const
{
    const INFO *data = getDataSource();
    device()->setCursor(0, 0);
    device()->print("RAM Info");
    device()->setCursor(0, 1);
    device()->print(String("  free : ") + String(data->memoryFree) + String(" "));
    switch (data->memoryFreePrescaler)
    {
    case PRESCALER::B:
        device()->print("B");
        break;
    case PRESCALER::KB:
        device()->print("KB");
        break;
    case PRESCALER::MB:
        device()->print("MB");
        break;
    case PRESCALER::GB:
        device()->print("GB");
        break;
    case PRESCALER::TB:
        device()->print("TB");
        break;
    case PRESCALER::PB:
        device()->print("PB");
        break;
    }
    device()->setCursor(0, 2);
    device()->print(String("  total: ") + String(data->memoryTotal) + String(" "));
    switch (data->memoryTotalPrescaler)
    {
    case PRESCALER::B:
        device()->print("B");
        break;
    case PRESCALER::KB:
        device()->print("KB");
        break;
    case PRESCALER::MB:
        device()->print("MB");
        break;
    case PRESCALER::GB:
        device()->print("GB");
        break;
    case PRESCALER::TB:
        device()->print("TB");
        break;
    case PRESCALER::PB:
        device()->print("PB");
        break;
    }
}