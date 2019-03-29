#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "data.h"
#include "cpu_screen.h"
#include "cpu_graph_screen.h"
#include "ram_screen.h"
#include "hdd_screen.h"

INFO __visible_data;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
AbstractScreen *screen;
CPUScreen cpu_screen(&lcd, 20, 4, &__visible_data);
RAMScreen ram_screen(&lcd, 20, 4, &__visible_data);
HDDScreen hdd_screen(&lcd, 20, 4, &__visible_data);
CPUGraphScreen cpu_graph_screen(&lcd, 20, 4, &__visible_data);

void setup()
{
  Serial.begin(57600);
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print(sizeof(INFO));
  delay(1000);
  screen = &cpu_graph_screen;
}

void loop()
{
  while (!getInfo())
    ;

  displayInfo();
}

bool getInfo()
{
  if (Serial.available() <= 0)
    return false;

  delay(100);
  char buffer[sizeof(INFO)];
  Serial.readBytes(buffer, sizeof(INFO));
  __visible_data = *(INFO *)(buffer);
  if (__visible_data.signature[0] != 'I' ||
      __visible_data.signature[1] != 'N' ||
      __visible_data.signature[2] != 'F' ||
      (__visible_data.signature[3] != 'O' && __visible_data.signature[3] != '2'))
    return false;

  return true;
}

void displayInfo()
{
  lcd.clear();
  screen->draw();
}