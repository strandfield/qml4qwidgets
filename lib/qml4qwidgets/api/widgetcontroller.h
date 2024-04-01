
#ifndef Q4Q_WIDGETCONTROLLER_H
#define Q4Q_WIDGETCONTROLLER_H

#include "controller.h"

class QWidget;

class Q4Q_API WidgetController : public Controller
{
  Q_OBJECT
  Q_PROPERTY(QWidget* controlledWidget READ controlledWidget NOTIFY controlledWidgetChanged)
public:
  explicit WidgetController(QObject* parent = nullptr);
  ~WidgetController();

  QWidget* controlledWidget() const;
  void setControlledWidget(QWidget* w);

Q_SIGNALS:
  void controlledWidgetChanged();

private:
  QWidget* m_widget = nullptr;
};

#endif // Q4Q_WIDGETCONTROLLER_H
