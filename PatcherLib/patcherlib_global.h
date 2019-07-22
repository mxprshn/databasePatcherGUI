#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PATCHERLIB_LIB)
#  define PATCHERLIB_EXPORT Q_DECL_EXPORT
# else
#  define PATCHERLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define PATCHERLIB_EXPORT
#endif
