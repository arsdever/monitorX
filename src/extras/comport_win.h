#pragma once

#include "comport.h"
#include "ordered_map.h"

#include <windows.h>
#include <string>

class ComportWindows : public Comport
{
public:
	ComportWindows(std::string const &port_name, int baudrate = 9600, int bytesize = 8, int stopbits = 1, int parity = 0, int dtrcontrol = DTR_CONTROL_ENABLE);
	~ComportWindows() override;

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
	bool __is_valid : 1;
	HANDLE __handle;
	DCB __default_settings;
	bool __opened : 1;

public:
	static void updateComMap();
	static OrderedMap<QString, QString> s_comMap;
};
