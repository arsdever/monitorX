#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PROVIDER_LIB)
#  define PROVIDER_EXPORT Q_DECL_EXPORT
# else
#  define PROVIDER_EXPORT Q_DECL_IMPORT
# endif
#else
# define PROVIDER_EXPORT
#endif
