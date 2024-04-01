#include "widgetcontroller.h"

WidgetController::WidgetController(QObject* parent) : Controller(parent)
{

}

WidgetController::~WidgetController()
{

}

QWidget* WidgetController::controlledWidget() const
{
  return m_widget;
}

void WidgetController::setControlledWidget(QWidget* w)
{
  if (m_widget != w) {
    m_widget = w;
    Q_EMIT controlledWidgetChanged();
  }
}
