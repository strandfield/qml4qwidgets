
#ifndef Q4Q_WIDGETCONTROLLER_H
#define Q4Q_WIDGETCONTROLLER_H

#include "controller.h"

class Q4Q_API WidgetController : public Controller
{
  Q_OBJECT
public:
  explicit WidgetController(QObject* parent = nullptr);
  ~WidgetController();
};

#endif // Q4Q_WIDGETCONTROLLER_H
