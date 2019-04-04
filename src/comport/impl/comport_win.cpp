#include "comport_win.h"

#include <QByteArray>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

OrderedMap< QString, QString > ComportWindows::s_com_map;

ComportWindows::ComportWindows(std::string const &port_name, int baudrate, int bytesize, int stopbits, int parity, int dtrcontrol)
	: __port_name(port_name)
	  , __is_valid(false)
	  , __handle(NULL)
	  , __default_settings{0}
	  , __opened(false)
{
	__default_settings.DCBlength = sizeof(DCB);
	__default_settings.BaudRate = baudrate;
	__default_settings.ByteSize = bytesize;
	__default_settings.StopBits = stopbits;
	__default_settings.Parity = parity;
	__default_settings.fDtrControl = dtrcontrol;
}

ComportWindows::~ComportWindows() { close_port(); }

bool ComportWindows::is_opened() const { return __opened; }

void ComportWindows::open_port()
{
	__handle = CreateFile(__port_name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if ( __handle == INVALID_HANDLE_VALUE )
		__is_valid = false;
	else
	{
		__is_valid = true;
		__opened = true;
		configure();
	}
}

bool ComportWindows::configure() const
{
	DCB settings;
	memset(&settings, 0, sizeof(DCB));
	settings.DCBlength = sizeof(DCB);
	GetCommState(__handle, &settings);
	settings.DCBlength = __default_settings.DCBlength;
	settings.BaudRate = __default_settings.BaudRate;
	settings.ByteSize = __default_settings.ByteSize;
	settings.StopBits = __default_settings.StopBits;
	settings.Parity = __default_settings.Parity;
	settings.fDtrControl = __default_settings.fDtrControl;
	return SetCommState(__handle, &settings);
}

void ComportWindows::close_port()
{
	if ( __is_valid )
	{
		CloseHandle(__handle);
		__opened = false;
	}
}

uint32_t ComportWindows::write_data(QByteArray const &data)
{
	DWORD written;
	WriteFile(__handle, data, data.size(), &written, nullptr);
	return static_cast< uint32_t >(written);
}

uint32_t ComportWindows::write_data(const char *data, const uint32_t size)
{
	DWORD written;
	WriteFile(__handle, data, size, &written, nullptr);
	return static_cast< uint32_t >(written);
}

uint32_t ComportWindows::read_data(QByteArray &data)
{
	DWORD bytes_read;
	ReadFile(__handle, data.data(), data.size(), &bytes_read, nullptr);
	return static_cast< uint32_t >(bytes_read);
}

uint32_t ComportWindows::read_data(char *&data, uint32_t size)
{
	DWORD bytes_read;
	data = new char[size];
	ReadFile(__handle, data, size, &bytes_read, nullptr);
	return static_cast< uint32_t >(bytes_read);
}

void ComportWindows::updateComMap()
{
	int32_t buffer_size = 256;
	char *result = new char[buffer_size];
	for ( int i = 0; i < 256; ++i )
	{
		QString com_name = QString("COM%1").arg(i);
		DWORD const response_length = QueryDosDevice(com_name.toLatin1(), result, buffer_size);

		if ( response_length != 0 )
		{
			QString path = QString::fromLatin1(result);
			s_com_map[com_name] = path;
		}

		if ( ::GetLastError() == ERROR_INSUFFICIENT_BUFFER )
		{
			buffer_size *= 2;
			delete[] result;
			result = new char[buffer_size];
			--i;
			continue;
		}
	}
	delete[] result;
}
