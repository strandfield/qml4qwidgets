#include "factory.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <QDebug>

QuarkWidgetFactory::~QuarkWidgetFactory()
{

}

QWidget* QuarkWidgetFactory::createWidget(const QString& className, QWidget *parent)
{
  if (uiLoader.availableWidgets().contains(className)) {
    return uiLoader.createWidget(className, parent);
  }

  QString path = uiLoader.workingDirectory().absoluteFilePath(className + ".ui");
  QFile file{ path };

  if (!file.exists()) {
    return nullptr;
  }

  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "failed to open" << path;
    return nullptr;
  }

  return uiLoader.load(&file, parent);
}
