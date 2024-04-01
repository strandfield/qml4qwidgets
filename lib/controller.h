
#ifndef Q4Q_CONTROLLER_H
#define Q4Q_CONTROLLER_H

#include "qml4qwidgets.defs.h"

#include <QObject>

#include <QList>
#include <QQmlListProperty>

class Q4Q_API Controller : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QQmlListProperty<QObject> data READ data NOTIFY dataChanged)
  Q_CLASSINFO("DefaultProperty", "data")
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller();

  QList<QObject*>& dataref();
  QQmlListProperty<QObject> data();

Q_SIGNALS:
  void dataChanged();

private:
  QList<QObject*> m_data;
};

#endif // Q4Q_CONTROLLER_H
