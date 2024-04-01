
#ifndef Q4Q_QML4QWIDGETS_H
#define Q4Q_QML4QWIDGETS_H

#include "qml4qwidgets.defs.h"

#include <QObject>

#include <map>

class QQmlComponent;
class QQmlContext;
class QQmlEngine;
class QWidget;

class Q4Q_API Qml4QWidgetsController : public QObject
{
  Q_OBJECT
public:
  explicit Qml4QWidgetsController(QQmlEngine* qmlEngine, QObject* parent = nullptr);
  ~Qml4QWidgetsController();

  QQmlEngine* qmlEngine() const;

  static void collectNamedObjects(QObject* obj, std::map<QString, QObject*>& outmap);
  static std::map<QString, QObject*> collectNamedObjects(QObject* obj);

  Q_INVOKABLE void destroyWidget(QWidget* w);

  Q_INVOKABLE void installControllerOnWidget(QWidget* widget, QQmlComponent* component);
  Q_INVOKABLE QObject* getControllerForWidget(QWidget* widget) const;
  Q_INVOKABLE bool hasControllerForWidget(QWidget* widget) const;

protected:
  void setupController(QWidget* widget, QQmlComponent* component);
  QQmlContext* createQmlContextForWidget(QWidget* widget);

private:
  QQmlEngine* m_qml_engine;
  std::map<QWidget*, QObject*> m_widgetcontrollers;
};

Q4Q_API void registerQml4QWidgetsTypes();

#endif // Q4Q_QML4QWIDGETS_H
