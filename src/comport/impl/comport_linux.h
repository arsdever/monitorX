#pragma once

#include "../comport.h"
#include "../ordered_map.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define DEFAULT_PATH "/dev/"

class ComportLinux : public Comport
{
public:
  ComportLinux(std::string const &port_name, int baudrate = 9600, int bytesize = 8, int stopbits = 1, int parity = 0);
  ~ComportLinux() override;

  uint32_t write_data(QByteArray const &data) override;
  uint32_t write_data(const char *data, uint32_t size) override;
  uint32_t read_data(QByteArray &data) override;
  uint32_t read_data(char *&data, uint32_t size) override;

  bool is_opened() const override;

  void open_port() override;
  void close_port() override;

private:
  bool configurate();

private:
  std::string __port_name;
  int __descriptor;
  Settings __default_settings;
  bool __is_valid : 1;
  bool __opened : 1;

public:
  static void updateComMap();
  static OrderedMap<QString, QString> s_com_map;
};