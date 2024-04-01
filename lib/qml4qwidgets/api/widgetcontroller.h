
#ifndef Q4Q_WIDGETCONTROLLER_H
#define Q4Q_WIDGETCONTROLLER_H

#include "controller.h"

class QWidget;

/**
 * \brief C++ implementation of the QML WidgetController type
 *
 * This type, which derives from Controller, is meant to be used
 * as a QML controller for a QWidget.
 *
 * \sa Qml4QWidgetsController::installControllerOnWidget().
 */
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
