#pragma once

#include "extras_global.h"
#include "ordered_map.h"

#include <windows.h>
#include <string>

class QMenuBar;
class QByteArray;

class EXTRAS_EXPORT Comport
{
public:
	Comport(std::string const& port_name, int baudrate = 9600, int bytesize = 8, int stopbits = 1, int parity = 0, int dtrcontrol = DTR_CONTROL_ENABLE);
	~Comport();

	unsigned __int32 write_data(QByteArray const &data);
	unsigned __int32 write_data(const char *data, unsigned __int32 size);
	unsigned __int32 read_data(QByteArray &data);
	unsigned __int32 read_data(char *&data, unsigned __int32 size);

private:
	void open_port();
	void configurate();
	void close_port();

private:
	std::string __port_name;
	bool __is_valid : 1;
	HANDLE __handle;
	DCB __default_settings;

public:
	static void updateComMap();
	static OrderedMap<QString, QString> s_comMap;
};

extern "C" EXTRAS_EXPORT void addMenu(QMenuBar *menuBar);
