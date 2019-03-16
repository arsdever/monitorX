#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GRAPHER_LIB)
#  define GRAPHER_EXPORT Q_DECL_EXPORT
# else
#  define GRAPHER_EXPORT Q_DECL_IMPORT
# endif
#else
# define GRAPHER_EXPORT
#endif
