#include "messageboxapi.h"

#include <QMessageBox>

QmlMessageBoxAPI::QmlMessageBoxAPI(QObject *parent) : QObject(parent)
{

}

QmlMessageBoxAPI::~QmlMessageBoxAPI()
{

}

void QmlMessageBoxAPI::about(QWidget* parent, const QString& title, const QString& text)
{
  QMessageBox::about(parent, title, text);
}

void QmlMessageBoxAPI::aboutQt(QWidget* parent, const QString& title)
{
  QMessageBox::aboutQt(parent, title);
}

void QmlMessageBoxAPI::information(QWidget* parent, const QString& title, const QString& text)
{
  QMessageBox::information(parent, title, text);
}
