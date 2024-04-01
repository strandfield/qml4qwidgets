
#ifndef QUARK_FACTORY_H
#define QUARK_FACTORY_H

#include "qml4qwidgets/widgetfactory.h"

#include <QUiLoader>

class QuarkWidgetFactory : public WidgetFactory
{
public:
  QUiLoader uiLoader;

public:
  ~QuarkWidgetFactory();

  QWidget* createWidget(const QString& className, QWidget *parent) override;
};

#endif // QUARK_FACTORY_H
