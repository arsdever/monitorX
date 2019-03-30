#pragma once

#include "extras_global.h"

#include <stdint.h>
#include <string>

class EXTRAS_EXPORT Comport
{
public:
  struct Settings
  {
    std::string name;
    uint32_t baudrate;
    uint32_t bytesize;
    uint32_t stopbits;
    uint32_t parity;
  };

public:
  virtual ~Comport() {}

  virtual uint32_t write_data(QByteArray const &data) = 0;
  virtual uint32_t write_data(const char *data, uint32_t size) = 0;
  virtual uint32_t read_data(QByteArray &data) = 0;
  virtual uint32_t read_data(char *&data, uint32_t size) = 0;

  virtual bool is_opened() const = 0;

  virtual void open_port() = 0;
  virtual void close_port() = 0;
};