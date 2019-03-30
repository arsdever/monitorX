#include "comport.h"

#include <QByteArray>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

OrderedMap<QString, QString> Comport::s_comMap;

Comport::Comport(std::string const& port_name, int baudrate, int bytesize, int stopbits, int parity, int dtrcontrol)
	: __port_name(port_name)
	, __is_valid(false)
	, __default_settings{ 0 }
	, __opened(false)
{
	__default_settings.DCBlength = sizeof(DCB);
	__default_settings.BaudRate = baudrate;
	__default_settings.ByteSize = bytesize;
	__default_settings.StopBits = stopbits;
	__default_settings.Parity = parity;
	__default_settings.fDtrControl = dtrcontrol;
}

Comport::~Comport()
{
	close_port();
}

bool Comport::is_opened() const
{
	return __opened;
}

void Comport::open_port()
{
	__handle = CreateFile(__port_name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (__handle == INVALID_HANDLE_VALUE)
		__is_valid = false;
	else
	{
		__is_valid = true;
		__opened = true;
		configurate();
	}
}

bool Comport::configurate()
{
	DCB settings{ 0 };
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

void Comport::close_port()
{
	if (__is_valid)
	{
		CloseHandle(__handle);
		__opened = false;
	}
}

uint32_t Comport::write_data(QByteArray const &data)
{
	DWORD written;
	WriteFile(__handle, data, data.size(), &written, 0);
	return (uint32_t)written;
}

uint32_t Comport::write_data(const char *data, uint32_t size)
{
	DWORD written;
	WriteFile(__handle, data, size, &written, 0);
	return (uint32_t)written;
}

uint32_t Comport::read_data(QByteArray &data)
{
	DWORD readen;
	ReadFile(__handle, data.data(), data.size(), &readen, 0);
	return (uint32_t)readen;
}

uint32_t Comport::read_data(char *&data, uint32_t size)
{
	DWORD readen;
	data = new char[size];
	ReadFile(__handle, data, size, &readen, 0);
	return (uint32_t)readen;
}

void Comport::updateComMap()
{
	int32_t buffer_size = 256;
	char* result = new char[buffer_size];
	for (int i = 0; i < 256; ++i)
	{
		QString comName = QString("COM%1").arg(i);
		DWORD responseLength = QueryDosDevice(comName.toLatin1(), result, buffer_size);

		if (responseLength != 0)
		{
			QString path = QString::fromLatin1(result);
			s_comMap[comName] = path;
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			buffer_size *= 2;
			delete result;
			result = new char[buffer_size];
			--i;
			continue;
		}
	}
	delete result;
}

extern "C" EXTRAS_EXPORT void addMenu(QMenuBar *menuBar)
{
	if (menuBar == nullptr)
		return;

	QList<QMenu*> menuList = menuBar->findChildren<QMenu*>();
	QMenu* comMenu = nullptr;
	for (QMenu* menu : menuList)
	{
		if (menu->title() != "Connect")
			continue;

		comMenu = menu;
		break;
	}

	if (comMenu == nullptr)
	{
		comMenu = menuBar->addMenu("Connect");
	}
	else
	{
		comMenu->deleteLater();
		menuBar->addMenu("Connect");
	}

	Comport::updateComMap();
	if (Comport::s_comMap.empty())
	{
		comMenu->addAction("No devices available")->setEnabled(false);
	}
	else
	{
		QList<QString> keys = Comport::s_comMap.keys();
		for (QString comport : keys)
		{
			QAction* pAction = comMenu->addAction(comport);
		}
	}
}
extern "C" EXTRAS_EXPORT Comport* openPort(QString const& portName)
{
	return new Comport(portName.toStdString(), 57600);
}