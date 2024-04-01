#pragma once

#if (defined(WIN32) || defined(_WIN32)) && !defined(QML4QWIDGETS_STATIC_LINKING)

#if defined(QML4QWIDGETS_BUILD_LIBRARY)
#define Q4Q_API __declspec(dllexport)
#else
#define Q4Q_API __declspec(dllimport)
#endif

#else

#define Q4Q_API

#endif
