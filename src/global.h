#ifndef MERE_XDG_GLOBAL_H
#define MERE_XDG_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(MERE_XDG_LIB)
#  define MERE_XDG_LIB_SPEC Q_DECL_EXPORT
#else
#  define MERE_XDG_LIB_SPEC Q_DECL_IMPORT
#endif


#endif // MERE_XDG_GLOBAL_H
