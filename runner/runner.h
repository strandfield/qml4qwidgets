
#ifndef QUARK_RUNNER_H
#define QUARK_RUNNER_H

#include "qml4qwidgets/qml4qwidgets.h"

#include <QObject>

class QuarkWidgetFactory;

class QuarkRunner : public QObject
{
public:
  explicit QuarkRunner(QObject* parent = nullptr);
  ~QuarkRunner();

  bool setupFromCommandLine(const QStringList& args);

  void setWidget(const QString& formUiPath);
  void setController(const QString& qmlControllerFilePath);
  void setController(const QUrl& url);

protected:
  void setupController(QQmlComponent* component);

protected Q_SLOTS:
  void onFileChanged(const QString& filePath);

private:
  Qml4QWidgetsController* m_q4q_controller;
  QuarkWidgetFactory* m_factory = nullptr;
  QWidget* m_widget = nullptr;
  QQmlComponent* m_component = nullptr;
  QObject* m_controller = nullptr;
};

#endif // QUARK_RUNNER_H
