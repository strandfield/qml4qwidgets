#include "qml4qwidgets.h"

#include "widgetcontroller.h"

#include "messageboxapi.h"

#include <QQmlEngine>

void initQml4QWidgets()
{
  qmlRegisterType<Controller>("Qml4QWidgets", 1, 0, "Controller");
  qmlRegisterType<WidgetController>("Qml4QWidgets", 1, 0, "WidgetController");

  qmlRegisterSingletonType("Qml4QWidgets", 1, 0, "MessageBox", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QJSValue {
    Q_UNUSED(engine)
    return scriptEngine->newQObject(new QmlMessageBoxAPI(engine));
  });
}
