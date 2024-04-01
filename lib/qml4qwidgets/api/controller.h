
#ifndef Q4Q_CONTROLLER_H
#define Q4Q_CONTROLLER_H

#include "qml4qwidgets/qml4qwidgets.defs.h"

#include <QObject>

#include <QList>
#include <QQmlListProperty>

/**
 * \brief C++ implementation of the QML Controller type
 *
 * This type is similar to the QtObject type from the QtQml module.
 * It defines an additional default property named "data" which
 * effectively adds support for child objects.
 *
 * \code[qml]
 * import QtQml 2.15
 * import Qml4QWidget 1.0
 *
 * Controller {
 *   id: root
 *
 *   Connections {
 *     // more code
 *   }
 * }
 * \endcode
 */
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
