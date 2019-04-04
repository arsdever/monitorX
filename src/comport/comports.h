#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include "./impl/comport_win.h"
#define COMPORT_CLASS ComportWindows
#elif defined(__linux__)
#include "./impl/comport_linux.h"
#define COMPORT_CLASS ComportLinux
#endif
