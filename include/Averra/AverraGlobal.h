#ifndef AVERRA_GLOBAL_H
#define AVERRA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AVERRA_BUILD_LIBRARY)
#define AVERRA_EXPORT Q_DECL_EXPORT
#else
#define AVERRA_EXPORT Q_DECL_IMPORT
#endif

#endif // AVERRA_GLOBAL_H

