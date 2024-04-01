
#ifndef Q4Q_API_MESSAGEBOX_H
#define Q4Q_API_MESSAGEBOX_H

#include "qml4qwidgets.defs.h"

#include <QObject>

class Q4Q_API QmlMessageBoxAPI : public QObject
{
  Q_OBJECT
public:
  explicit QmlMessageBoxAPI(QObject* parent = nullptr);
  ~QmlMessageBoxAPI();

  Q_INVOKABLE void about(QWidget* parent, const QString& title, const QString& text);
  Q_INVOKABLE void aboutQt(QWidget* parent, const QString& title = QString());
  Q_INVOKABLE void information(QWidget* parent, const QString& title, const QString& text);
};

#endif // Q4Q_API_MESSAGEBOX_H
