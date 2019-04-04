#include "comport_linux.h"

#include <QByteArray>
#include <QMenuBar>
#include <QAction>

ComportLinux::ComportLinux(std::string const &port_name, int baudrate, int bytesize, int stopbits, int parity)
    : __port_name(port_name), __descriptor(-1), __is_valid(false), __opened(false)
{
    __default_settings.name = port_name;
    __default_settings.baudrate = baudrate;
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

extern "C" EXTRAS_EXPORT void addMenu(QMenuBar *menuBar)
{
    if (menuBar == nullptr)
        return;

    QList<QAction *> actionsList = menuBar->findChildren<QAction *>();
    QAction *connectAction = nullptr;
    for (QAction *action : actionsList)
    {
        if (action->text() != "Connect")
            continue;

        connectAction = action;
        break;
    }

    if (connectAction == nullptr)
        menuBar->addAction("Connect");
}

extern "C" EXTRAS_EXPORT ComportLinux *openPort(QString const &portName)
{
    return new ComportLinux(portName.toStdString(), B57600);
}