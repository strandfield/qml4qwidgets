#include "runner.h"

#include "qml4qwidgets.h"

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

void listNamedObjects(QObject* obj, std::map<QString, QObject*>& outmap)
{
  for (QObject* o : obj->children()) {
    listNamedObjects(o, outmap);
  }

  if (!obj->objectName().isEmpty()) {
    outmap[obj->objectName()] = obj;
  }
}

std::map<QString, QObject*> listNamedObjects(QObject* obj)
{
  std::map<QString, QObject*> dict;
  listNamedObjects(obj, dict);
  return dict;
}

QuarkRunner::QuarkRunner(QObject* parent) : QObject(parent)
{

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
    m_widget->close();
    m_widget->deleteLater();
  }

  m_widget = widget;

  if (m_component) {
    setupController(m_component);
  }
}

void QuarkRunner::setController(const QString& qmlControllerFilePath)
{
  setController(QUrl::fromLocalFile(qmlControllerFilePath));
}

void QuarkRunner::setController(const QUrl& url)
{
  if (!m_qml_engine) {
    initQmlEngine();
  }

  if (m_component && m_component->url() == url) {
    m_qml_engine->clearComponentCache();
  }

  auto* component = new QQmlComponent(m_qml_engine);
  connect(component, &QQmlComponent::statusChanged, this, &QuarkRunner::onQmlComponentStatusChanged);
  component->loadUrl(url, QQmlComponent::Asynchronous);
}

void QuarkRunner::initQmlEngine()
{
  if (m_qml_engine) {
    return;
  }

  initQml4QWidgets();

  m_qml_engine = new QQmlEngine(this);
}

QQmlContext* QuarkRunner::createQmlContextForWidget(QWidget* widget)
{
  std::map<QString, QObject*> objs = listNamedObjects(widget);

  auto* context = new QQmlContext(m_qml_engine->rootContext(), widget);

  for (auto p : objs) {
    context->setContextProperty(p.first, p.second);
  }

  context->setContextProperty("widget", widget);

  return context;
}

void QuarkRunner::setupController(QQmlComponent* component)
{
  QQmlContext* context = createQmlContextForWidget(m_widget);

  QObject* controller = component->beginCreate(context);

  if (!controller) {
    return;
  }

  if (m_controller) {
    m_controller->deleteLater();
  }

  m_controller = controller;

  m_controller->setParent(m_widget);

  component->completeCreate();

  m_component = component;
}

void QuarkRunner::onQmlComponentStatusChanged()
{
  auto* component = qobject_cast<QQmlComponent*>(sender());

  if (!component) {
    return;
  }

  if (component->status() == QQmlComponent::Error) {
    qDebug() << "QQmlComponent compilation failed: " << component->errorString();
  } else if (component->status() == QQmlComponent::Loading) {
    qDebug() << "QQmlComponent is loading...";
  }

  if (component->status() != QQmlComponent::Ready) {
    return;
  }

  setupController(component);
}

void QuarkRunner::onFileChanged(const QString& filePath)
{
  // TODO: add a delay before reloading the ui,
  // or maybe wait for the application to gain focus again?

  if (filePath.endsWith(".ui")) {
    setWidget(filePath);
  } else if (filePath.endsWith(".qml")) {
    setController(filePath);
  }

  auto* watcher = qobject_cast<QFileSystemWatcher*>(sender());
  if (watcher) {
    if (!watcher->files().contains(filePath)) {
      watcher->addPath(filePath);
    }
  }
}
