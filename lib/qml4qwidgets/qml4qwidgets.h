
#ifndef Q4Q_QML4QWIDGETS_H
#define Q4Q_QML4QWIDGETS_H

#include "qml4qwidgets.defs.h"

#include <QObject>

#include <map>
#include <memory>

class QQmlComponent;
class QQmlContext;
class QQmlEngine;
class QWidget;

class WidgetFactory;

/**
 * \brief main entry point for the Qml4QWidgets C++ API
 *
 * This class can be used to attach a QML controller to any QWidget.
 * It can be used both from C++ and QML.
 *
 * QML controllers instantiated with an instance of this class have
 * access to the instance through a "qml4qwidgets" context variable.
 */
class Q4Q_API Qml4QWidgetsController : public QObject
{
  Q_OBJECT
public:
  explicit Qml4QWidgetsController(QQmlEngine* qmlEngine, QObject* parent = nullptr);
  ~Qml4QWidgetsController();

  QQmlEngine* qmlEngine() const;

  static void collectNamedObjects(QObject* obj, std::map<QString, QObject*>& outmap);
  static std::map<QString, QObject*> collectNamedObjects(QObject* obj);

  WidgetFactory* getWidgetFactory() const;
  void setWidgetFactory(std::unique_ptr<WidgetFactory> factory);

  Q_INVOKABLE QWidget* createWidget(const QString& className, QWidget* parent = nullptr, const QString& name = QString());
  Q_INVOKABLE void destroyWidget(QWidget* w);

  Q_INVOKABLE void installControllerOnWidget(QWidget* widget, QQmlComponent* component);
  Q_INVOKABLE QObject* getControllerForWidget(QWidget* widget) const;
  Q_INVOKABLE bool hasControllerForWidget(QWidget* widget) const;
  Q_INVOKABLE void removeControllerOnWidget(QWidget* widget);

protected:
  void setupController(QWidget* widget, QQmlComponent* component);
  QQmlContext* createQmlContextForWidget(QWidget* widget);

private:
  QQmlEngine* m_qml_engine;
  std::unique_ptr<WidgetFactory> m_widget_factory;
  std::map<QWidget*, QObject*> m_widgetcontrollers;
};

Q4Q_API void registerQml4QWidgetsTypes();

#endif // Q4Q_QML4QWIDGETS_H
