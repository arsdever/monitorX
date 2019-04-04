#include "abstract_screen_decorator.h"

#include <WString.h>

class InfoScreen;

class DateDecoratedScreen : public AbstractScreenDecorator
{
public:
  DateDecoratedScreen(InfoScreen *screen);

  void draw() const override;
  String dateToString() const;

  inline String normalize(uint8_t number) const { return (number > 9) ? String(number) : String('0') + String(number); }
  inline String normalizeYear(uint16_t year) const { return (year > 1000) ? String(year) : ((year > 100) ? String('0') + String(year) : ((year > 10) ? String("00") + String(year) : String("000") + String(year))); }
};