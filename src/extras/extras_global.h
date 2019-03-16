#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EXTRAS_LIB)
#  define EXTRAS_EXPORT Q_DECL_EXPORT
# else
#  define EXTRAS_EXPORT Q_DECL_IMPORT
# endif
#else
# define EXTRAS_EXPORT
#endif
