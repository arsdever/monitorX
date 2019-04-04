#include "comport_linux.h"

#include <QByteArray>
#include <QMenuBar>
#include <QAction>
#include <QDir>

OrderedMap<QString, QString> ComportLinux::s_com_map;

ComportLinux::ComportLinux(std::string const &port_name, int baudrate, int bytesize, int stopbits, int parity)
    : __port_name(DEFAULT_PATH + port_name), __descriptor(-1), __is_valid(false), __opened(false)
{
    __default_settings.name = port_name;
    switch (baudrate)
    {
    case 0:
        __default_settings.baudrate = B0;
        break;
    case 50:
        __default_settings.baudrate = B50;
        break;
    case 75:
        __default_settings.baudrate = B75;
        break;
    case 110:
        __default_settings.baudrate = B110;
        break;
    case 134:
        __default_settings.baudrate = B134;
        break;
    case 150:
        __default_settings.baudrate = B150;
        break;
    case 200:
        __default_settings.baudrate = B200;
        break;
    case 300:
        __default_settings.baudrate = B300;
        break;
    case 600:
        __default_settings.baudrate = B600;
        break;
    case 1200:
        __default_settings.baudrate = B1200;
        break;
    case 1800:
        __default_settings.baudrate = B1800;
        break;
    case 2400:
        __default_settings.baudrate = B2400;
        break;
    case 4800:
        __default_settings.baudrate = B4800;
        break;
    case 9600:
        __default_settings.baudrate = B9600;
        break;
    case 19200:
        __default_settings.baudrate = B19200;
        break;
    case 38400:
        __default_settings.baudrate = B38400;
        break;
    case 57600:
        __default_settings.baudrate = B57600;
        break;
    case 115200:
        __default_settings.baudrate = B115200;
        break;
    case 230400:
        __default_settings.baudrate = B230400;
        break;
    default:
        __default_settings.baudrate = B9600;
    }
    __default_settings.bytesize = bytesize;
    __default_settings.stopbits = stopbits;
    __default_settings.parity = parity;
}

ComportLinux::~ComportLinux()
{
    close_port();
}

bool ComportLinux::is_opened() const
{
    return __opened;
}

void ComportLinux::open_port()
{
    __descriptor = open(__port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (__descriptor < 0)
    {
        __is_valid = false;
        __opened = false;
        return;
    }

    __is_valid = true;
    __opened = true;
    configurate();
}

bool ComportLinux::configurate()
{
    struct termios tty;
    memset(&tty, 0, sizeof(struct termios));

    if (tcgetattr(__descriptor, &tty) != 0)
    {
        __is_valid = false;
        return false;
    }

    switch (__default_settings.bytesize)
    {
    case 5:
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
        break;
    case 6:
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
        break;
    case 7:
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
        break;
    default:
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
        break;
    }

    cfsetospeed(&tty, __default_settings.baudrate);
    cfsetispeed(&tty, __default_settings.baudrate);

    tty.c_iflag &= IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= __default_settings.parity;

    if (__default_settings.stopbits > 1)
        tty.c_cflag |= CSTOPB;
    else
        tty.c_cflag &= ~CSTOPB;

    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(__descriptor, TCSANOW, &tty) != 0)
    {
        __is_valid = false;
        return false;
    }

    return true;
}

void ComportLinux::close_port()
{
    if (__is_valid)
    {
        close(__descriptor);
        __opened = false;
    }
}

uint32_t ComportLinux::write_data(QByteArray const &data)
{
    if (!__is_valid)
        return 0;

    return write(__descriptor, data.data(), data.size());
}

uint32_t ComportLinux::write_data(const char *data, uint32_t size)
{
    if (!__is_valid)
        return 0;

    return write(__descriptor, data, size);
}

uint32_t ComportLinux::read_data(QByteArray &data)
{
    if (!__is_valid)
        return 0;

    return read(__descriptor, data.data(), data.size());
}

uint32_t ComportLinux::read_data(char *&data, uint32_t size)
{
    if (!__is_valid)
        return 0;

    data = new char[size];
    return read(__descriptor, data, size);
}

void ComportLinux::updateComMap()
{
    s_com_map.clear();
    QDir directory("/dev/");
    QStringList devices = directory.entryList(QDir::System);

    for (QString device : devices)
    {
        if (device.mid(0, 3) == "tty")
            s_com_map[device] = device;
    }
}
