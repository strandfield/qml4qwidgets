#include "qml4qwidgets.h"

#include "widgetfactory.h"

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

/**
 * \brief construct a qml4qwidgets controller
 * \param qmlEngine  a pointer to the qml engine to use
 * \param parent     a pointer to the parent qobject
 *
 * If \a qmlEngine is nullptr, the controller will instantiate its own
 * QQmlEngine.
 */
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

/**
 * \brief returns a pointer to the qml engine
 */
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

/**
 * \brief recursively collect qobjects that have a non-empty objectName()
 * \param obj  root object
 * \return a map where the key is the objectName() of the qobject
 *
 * This function recursively processes \a obj and its children.
 * If several objects in the hierarchy have the same objectName(), only
 * the last one encountered will be accessible through the returned map.
 */
std::map<QString, QObject*> Qml4QWidgetsController::collectNamedObjects(QObject* obj)
{
  std::map<QString, QObject*> dict;
  collectNamedObjects(obj, dict);
  return dict;
}

/**
 * \brief returns the widget factory installed on the controller
 *
 * The factory can be used to create QWidgets both from C++ and QML using
 * the createWidget() method.
 * By default, no factory is installed and this function returns nullptr.
 */
WidgetFactory* Qml4QWidgetsController::getWidgetFactory() const
{
  return m_widget_factory.get();
}

/**
 * \brief install a widget factory on the controller
 * \param factory  a pointer to the factory
 *
 * The controller takes ownership of the factory.
 * Only one factory can be installed at a time, meaning \a factory
 * will replace any installed factory.
 * The currently installed factory, as returned by getWidgetFactory()
 * may be removed (and destroyed) by passing nullptr to this function.
 */
void Qml4QWidgetsController::setWidgetFactory(std::unique_ptr<WidgetFactory> factory)
{
  m_widget_factory = std::move(factory);
}

/**
 * \brief creates a widget using the currently installed factory
 * \param className  the class name of the widget
 * \param parent     an optional parent for the widget
 * \param name       an optional objectName() for the widget
 * \return a valid pointer to the create QWidget or nullptr on failure
 */
QWidget* Qml4QWidgetsController::createWidget(const QString& className, QWidget* parent, const QString& name)
{
  if (!getWidgetFactory()) {
    return nullptr;
  }

  QWidget* w = getWidgetFactory()->createWidget(className, parent);

  if (!w) {
    return w;
  }

  if (w->parentWidget() != parent) {
    w->setParent(parent);
  }

  w->setObjectName(name);

  return w;
}

/**
 * \brief destroys a widget
 * \param w  pointer to the widget
 *
 * This function takes care to destroy any QML controller installed on the
 * widget.
 *
 * The widget is not deleted immediately, its deletion is scheduled using deleteLater().
 */
void Qml4QWidgetsController::destroyWidget(QWidget* w)
{
  if (!w) {
    qDebug() << "null widget";
    return;
  }

  removeControllerOnWidget(w);

  w->setVisible(false);
  w->deleteLater();
}

/**
 * \brief install a controller on a widget
 * \param widget     pointer to the widget
 * \param component  pointer to the component from which the controller will be created
 *
 * If the \a component is not yet ready, the widget controller will be created
 * when \a component becomes ready.
 *
 * Installing a controller on a widget will remove any controller that was previously
 * installed.
 *
 * All objects in the children hierarchy of \a widget with a non-empty objectName() are
 * made available to the \a component as global context variables (using their objectName()
 * as the name of the variable).
 *
 * If the \a component creates an object derived from WidgetController (recommended),
 * the \a widget itself is also available as the controlledWidget() property of WidgetController.
 */
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

/**
 * \brief retrieves the controller of a widget
 * \param widget  a pointer to the widget
 * \return a valid pointer to the widget controller or nullptr if none was installed on the widget
 */
QObject* Qml4QWidgetsController::getControllerForWidget(QWidget* widget) const
{
  auto it = m_widgetcontrollers.find(widget);
  return it != m_widgetcontrollers.end() ? it->second : nullptr;
}

/**
 * \brief returns whether a controller is installed on a widget
 * \param widget  a pointer to the widget
 */
bool Qml4QWidgetsController::hasControllerForWidget(QWidget* widget) const
{
  return getControllerForWidget(widget) != nullptr;
}

/**
 * \brief removes any installed controller on a widget
 * \param widget  a pointer to the widget
 */
void Qml4QWidgetsController::removeControllerOnWidget(QWidget* widget)
{
  auto it = m_widgetcontrollers.find(widget);

  if (it != m_widgetcontrollers.end()) {
    it->second->deleteLater();
    m_widgetcontrollers.erase(it);
  }
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
