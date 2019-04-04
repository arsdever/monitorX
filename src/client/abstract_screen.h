#ifndef ABSTRACT_SCREEN_H
#define ABSTRACT_SCREEN_H

#include <stdint.h>

class LiquidCrystal;

class AbstractScreen
{
public:
  AbstractScreen(LiquidCrystal *lcd, uint8_t width, uint8_t height)
      : __device(lcd), __width(width), __height(height)
  {
  }
  virtual ~AbstractScreen(){};

  virtual LiquidCrystal *device() const { return __device; }

  virtual void draw() const = 0;
  virtual void scrollDown() = 0;
  virtual void scrollUp() = 0;

  virtual uint8_t width() const { return __width; }
  virtual uint8_t height() const { return __height; }

private:
  LiquidCrystal *__device;
  uint8_t __width;
  uint8_t __height;
};

#endif