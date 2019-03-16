#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COLLECTOR_LIB)
#  define COLLECTOR_EXPORT Q_DECL_EXPORT
# else
#  define COLLECTOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define COLLECTOR_EXPORT
#endif
