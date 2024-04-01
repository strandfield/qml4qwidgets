#include "runner.h"

#include "qml4qwidgets/qml4qwidgets.h"

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>

#include <QUiLoader>

#include <QWidget>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include <QDebug>

#include <map>

QuarkRunner::QuarkRunner(QObject* parent) : QObject(parent)
{
  m_q4q_controller = new Qml4QWidgetsController(nullptr, this);
}

QuarkRunner::~QuarkRunner()
{

}

bool QuarkRunner::setupFromCommandLine(const QStringList& args)
{
  QString formUiPath, qmlControllerPath;

  for (int i(0); i < args.size(); ++i)
  {
    if (args.at(i).startsWith("-")) {
      continue;
    }

    if (args.at(i).endsWith(".ui")) {
      formUiPath = args.at(i);
    } else if (args.at(i).endsWith(".qml")) {
      qmlControllerPath = args.at(i);
    }
  }

  if (formUiPath.isEmpty()) {
    qDebug() << "Missing .ui form";
    return false;
  }

  setWidget(formUiPath);

  if (!qmlControllerPath.isEmpty()) {
    setController(qmlControllerPath);
  } else {
    QFileInfo fileinfo{ formUiPath };
    QString qmlControllerFileName = fileinfo.baseName() + ".qml";
    fileinfo.setFile(fileinfo.dir(), qmlControllerFileName);
    if (fileinfo.exists()) {
      qmlControllerPath = fileinfo.absoluteFilePath();
      setController(qmlControllerPath);
    }
  }

  if (args.contains("--watch")) {
    auto* watcher = new QFileSystemWatcher(this);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &QuarkRunner::onFileChanged);
    watcher->addPath(formUiPath);
    if (!qmlControllerPath.isEmpty()) {
      watcher->addPath(qmlControllerPath);
    }

    qDebug() << "Watching files:" << watcher->files();
  }

  return true;
}

void QuarkRunner::setWidget(const QString& formUiPath)
{
  QFile file{ formUiPath };

  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "could not open " << formUiPath;
    return;
  }

  QUiLoader loader;
  
  QWidget* widget = loader.load(&file);

  if (!widget) {
    qDebug() << "failure to create widget: " << loader.errorString();
    return;
  }

  widget->show();

  if (m_widget) {
    m_q4q_controller->destroyWidget(m_widget);
  }

  m_widget = widget;

  if (m_component) {
    m_q4q_controller->installControllerOnWidget(m_widget, m_component);
  }
}

void QuarkRunner::setController(const QString& qmlControllerFilePath)
{
  setController(QUrl::fromLocalFile(qmlControllerFilePath));
}

void QuarkRunner::setController(const QUrl& url)
{
  m_q4q_controller->qmlEngine()->clearComponentCache();
  m_component = new QQmlComponent(m_q4q_controller->qmlEngine());
  m_component->loadUrl(url);

  if (m_widget) {
    m_q4q_controller->installControllerOnWidget(m_widget, m_component);
  }
}

void QuarkRunner::onFileChanged(const QString& filePath)
{
  // TODO: add a delay before reloading the ui,
  // or maybe wait for the application to gain focus again?

  if (filePath.endsWith(".ui")) {
    qDebug() << "detected changes on " << filePath << ", ui will be updated";
    setWidget(filePath);
  } else if (filePath.endsWith(".qml")) {
    qDebug() << "detected changes on " << filePath << ", controller will be updated";
    setController(filePath);
  }

  auto* watcher = qobject_cast<QFileSystemWatcher*>(sender());
  if (watcher) {
    if (!watcher->files().contains(filePath)) {
      watcher->addPath(filePath);
    }
  }
}
