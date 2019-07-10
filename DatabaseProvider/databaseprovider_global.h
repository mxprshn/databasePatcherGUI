#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DATABASEPROVIDER_LIB)
#  define DATABASEPROVIDER_EXPORT Q_DECL_EXPORT
# else
#  define DATABASEPROVIDER_EXPORT Q_DECL_IMPORT
# endif
#else
# define DATABASEPROVIDER_EXPORT
#endif
