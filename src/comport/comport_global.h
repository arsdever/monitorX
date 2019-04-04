#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMPORT_LIB)
#  define COMPORT_EXPORT Q_DECL_EXPORT
# else
#  define COMPORT_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMPORT_EXPORT
#endif
