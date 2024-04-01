
#ifndef QUARK_RUNNER_H
#define QUARK_RUNNER_H

#include <QObject>

#include <memory>

class QQmlComponent;
class QQmlContext;
class QQmlEngine;
class QWidget;

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
  void initQmlEngine();
  QQmlContext* createQmlContextForWidget(QWidget* widget);
  void setupController(QQmlComponent* component);

protected Q_SLOTS:
  void onQmlComponentStatusChanged();
  void onFileChanged(const QString& filePath);

private:
  QWidget* m_widget = nullptr;
  QQmlEngine* m_qml_engine = nullptr;
  QQmlComponent* m_component = nullptr;
  QObject* m_controller = nullptr;
};

#endif // QUARK_RUNNER_H
