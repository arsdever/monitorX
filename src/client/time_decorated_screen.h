#include "abstract_screen_decorator.h"

#include <WString.h>

class InfoScreen;

class TimeDecoratedScreen : public AbstractScreenDecorator
{
public:
  TimeDecoratedScreen(InfoScreen *screen);

  void draw() const override;
  String timeToString() const;
  void showSeconds(bool b = true);
  void hideSeconds(bool b = true);

  inline String normalize(uint8_t number) const { return (number > 9) ? String(number) : String('0') + String(number); }

private:
  bool __show_seconds;
};