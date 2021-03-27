#ifndef ACTORS_GLOBAL_H
#define ACTORS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACTORS_LIBRARY)
#  define ACTORS_EXPORT Q_DECL_EXPORT
#else
#  define ACTORS_EXPORT Q_DECL_IMPORT
#endif

#endif // ACTORS_GLOBAL_H
