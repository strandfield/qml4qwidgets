#include "qml4qwidgets.h"

#include "api/widgetcontroller.h"
#include "api/messageboxapi.h"

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>

#include <QWidget>

#include <QDebug>

void registerQml4QWidgetsTypes()
{
  qmlRegisterType<Controller>("Qml4QWidgets", 1, 0, "Controller");
  qmlRegisterType<WidgetController>("Qml4QWidgets", 1, 0, "WidgetController");

  qmlRegisterSingletonType("Qml4QWidgets", 1, 0, "MessageBox", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QJSValue {
    Q_UNUSED(engine)
    return scriptEngine->newQObject(new QmlMessageBoxAPI(engine));
  });
}

Qml4QWidgetsController::Qml4QWidgetsController(QQmlEngine* qmlEngine, QObject* parent) : QObject(parent),
  m_qml_engine(qmlEngine)
{
  if (!m_qml_engine) {
    m_qml_engine = new QQmlEngine(this);
  }

  registerQml4QWidgetsTypes();
}

Qml4QWidgetsController::~Qml4QWidgetsController()
{
  if (m_qml_engine->parent() == this) {
    delete m_qml_engine;
  }
}

QQmlEngine* Qml4QWidgetsController::qmlEngine() const
{
  return m_qml_engine;
}

void Qml4QWidgetsController::collectNamedObjects(QObject* obj, std::map<QString, QObject*>& outmap)
{
  for (QObject* o : obj->children()) {
    collectNamedObjects(o, outmap);
  }

  if (!obj->objectName().isEmpty()) {
    outmap[obj->objectName()] = obj;
  }
}

std::map<QString, QObject*> Qml4QWidgetsController::collectNamedObjects(QObject* obj)
{
  std::map<QString, QObject*> dict;
  collectNamedObjects(obj, dict);
  return dict;
}

void Qml4QWidgetsController::destroyWidget(QWidget* w)
{
  if (!w) {
    qDebug() << "null widget";
    return;
  }

  auto it = m_widgetcontrollers.find(w);

  if (it != m_widgetcontrollers.end()) {
    it->second->deleteLater();
    m_widgetcontrollers.erase(it);
  }

  w->setVisible(false);
  w->deleteLater();
}

void Qml4QWidgetsController::installControllerOnWidget(QWidget* widget, QQmlComponent* component)
{
  if (!widget) {
    qDebug() << "null widget";
    return;
  }

  if (!component) {
    qDebug() << "null component";
    return;
  }

  if (component->status() == QQmlComponent::Error) {
    qDebug() << "component has errors: " << component->errorString();
    return;
  }

  auto setup_controller = [this, widget, component](){
    setupController(widget, component);
  };

  if (component->status() == QQmlComponent::Ready) {
    setup_controller();
  } else {
    connect(component, &QQmlComponent::statusChanged, this, setup_controller);
  }
}

QObject* Qml4QWidgetsController::getControllerForWidget(QWidget* widget) const
{
  auto it = m_widgetcontrollers.find(widget);
  return it != m_widgetcontrollers.end() ? it->second : nullptr;
}

bool Qml4QWidgetsController::hasControllerForWidget(QWidget* widget) const
{
  return getControllerForWidget(widget) != nullptr;
}

void Qml4QWidgetsController::setupController(QWidget* widget, QQmlComponent* component)
{
  Q_ASSERT(widget && component);

  if (component->status() == QQmlComponent::Error) {
    qDebug() << "QQmlComponent compilation failed: " << component->errorString();
  } else if (component->status() == QQmlComponent::Loading) {
    return;
  }

  if (component->status() != QQmlComponent::Ready) {
    return;
  }

  QQmlContext* context = createQmlContextForWidget(widget);

  QObject* controller = component->beginCreate(context);

  if (!controller) {
    return;
  }

  QObject* previous = getControllerForWidget(widget);

  if (previous) {
    previous->deleteLater();
  }

  m_widgetcontrollers[widget] = controller;

  controller->setParent(qmlEngine());

  if (auto* wcon = qobject_cast<WidgetController*>(controller)) {
    wcon->setControlledWidget(widget);
  }

  component->completeCreate();
}

QQmlContext* Qml4QWidgetsController::createQmlContextForWidget(QWidget* widget)
{
  auto* context = new QQmlContext(qmlEngine()->rootContext(), widget);

  context->setContextProperty("qml4qwidgets", this);

  std::map<QString, QObject*> objs = collectNamedObjects(widget);
  for (auto p : objs) {
    context->setContextProperty(p.first, p.second);
  }

  return context;
}
