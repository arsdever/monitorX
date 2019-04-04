#include "abstract_screen.h"

class AbstractScreenDecorator : public AbstractScreen
{
public:
  AbstractScreenDecorator(AbstractScreen *screen);

  LiquidCrystal *device() const override;
  void draw() const override;
  void scrollDown() override;
  void scrollUp() override;

  uint8_t width() const override;
  uint8_t height() const override;

  AbstractScreen *getBaseScreen() const;

private:
  AbstractScreen *__screen;
};