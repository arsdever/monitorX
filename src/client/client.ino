#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "data.h"
#include "cpu_screen.h"
#include "cpu_graph_screen.h"
#include "ram_screen.h"
#include "hdd_screen.h"

INFO visible_data;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
AbstractScreen *screen;
CPUScreen cpu_screen(&lcd, 20, 4, &visible_data);
RAMScreen ram_screen(&lcd, 20, 4, &visible_data);
HDDScreen hdd_screen(&lcd, 20, 4, &visible_data);
CPUGraphScreen cpu_graph_screen(&lcd, 20, 4, &visible_data);

uint64_t last_data_input_point;

void setup()
{
  // initialize data signature for feature checks
  resetSignature();

  Serial.begin(57600);
  lcd.begin(20, 4);
  lcd.clear();
  String message("Waiting for data");
  lcd.setCursor(10 - message.length() / 2, 1);
  lcd.print(message);
  waitForConnection();
  message = String("Connected");
  lcd.clear();
  lcd.setCursor(10 - message.length() / 2, 1);
  lcd.print(message);
  screen = &cpu_graph_screen;
}

void loop()
{
  // wait for input data available and print
  if (getInfo())
  {
    displayInfo();
  }
  else // if data is unavailable up to 3 seconds, connection lost
      if (millis() - last_data_input_point > 3000)
  {
    lcd.clear();
    connectionLost();
  }
}

bool getInfo()
{
  // if no input available, don't wait
  if (Serial.available() <= 0)
    return false;

  // pin the time point
  last_data_input_point = millis();

  // if input available, wait 100 ms for finalization
  delay(100);

  // check if signature available
  if (!findSignature())
    return false;

  // read data and return
  return readData();
}

bool findSignature()
{
  // byte by byte read from input until getting sequence of "INF"
  uint8_t index = 0;
  char ch;
  while (Serial.available() > 0)
  {
    Serial.readBytes(&ch, 1);
    if (index < 3)
    {
      if (ch == visible_data.signature[index])
        ++index;
      else
      {
        index = ((ch == visible_data.signature[0]) ? 1 : 0);
      }
    }
    else
    {
      // the last character is the version of INF file
      visible_data.signature[index] = ch;
      return true;
    }
  }
  return false;
}

bool readData()
{
  char buffer[sizeof(INFO)];
  Serial.readBytes(buffer + 4, sizeof(INFO) - 4);

  buffer[0] = visible_data.signature[0];
  buffer[1] = visible_data.signature[1];
  buffer[2] = visible_data.signature[2];
  buffer[3] = visible_data.signature[3];

  visible_data = *(INFO *)(buffer);

  if (!checkSignature())
  {
    resetSignature();
    return false;
  }

  return true;
}

bool checkSignature()
{
  return visible_data.signature[0] == 'I' && visible_data.signature[1] == 'N' && visible_data.signature[2] == 'F' && (visible_data.signature[3] == 'O' || visible_data.signature[3] == '2');
}

void resetSignature()
{
  visible_data.signature[0] = 'I';
  visible_data.signature[1] = 'N';
  visible_data.signature[2] = 'F';
}

void connectionLost()
{
  String message("Connection lost");
  lcd.clear();
  lcd.setCursor(10 - message.length() / 2, 1);
  lcd.print(message);
  waitForConnection();
}

void waitForConnection()
{
  while (Serial.available() <= 0)
    ;
}

void displayInfo()
{
  lcd.clear();
  screen->draw();
}