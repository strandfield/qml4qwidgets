
#ifndef Q4Q_WIDGETFACTORY_H
#define Q4Q_WIDGETFACTORY_H

#include "qml4qwidgets.defs.h"

#include <QWidget>

/**
 * \brief abstract base class for a widget factory
 */
class Q4Q_API WidgetFactory
{
public:
  virtual ~WidgetFactory();

  /**
   * \brief creates a widget
   * \param className  class name of the QWidget
   * \param parent     optional parent widget
   * \return a valid pointer to a QWidget or nullptr on failure
   */
  virtual QWidget* createWidget(const QString& className, QWidget* parent = nullptr) = 0;
};

#endif // Q4Q_WIDGETFACTORY_H
