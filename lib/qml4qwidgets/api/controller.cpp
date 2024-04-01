#include "controller.h"

namespace
{

Controller* getController(QQmlListProperty<QObject>* list)
{
  return reinterpret_cast<Controller*>(list->data);
}

void append(QQmlListProperty<QObject>* list, QObject* obj)
{
  Controller* c= getController(list);
  c->dataref().append(obj);
  Q_EMIT c->dataChanged();
}

int count(QQmlListProperty<QObject>* list)
{
  Controller* c= getController(list);
  return c->dataref().size();
}

QObject* at(QQmlListProperty<QObject>* list, int n)
{
  Controller* c= getController(list);
  return c->dataref().at(n);
}

void clear(QQmlListProperty<QObject>* list)
{
  Controller* c= getController(list);
  c->dataref().clear();
  Q_EMIT c->dataChanged();
}

void replace(QQmlListProperty<QObject>* list, int n, QObject* val)
{
  Controller* c= getController(list);
  c->dataref().replace(n, val);
  Q_EMIT c->dataChanged();
}

void removelast(QQmlListProperty<QObject>* list)
{
  Controller* c= getController(list);
  c->dataref().removeLast();
  Q_EMIT c->dataChanged();
}

}

Controller::Controller(QObject* parent) : QObject(parent)
{

}

Controller::~Controller()
{

}

QList<QObject*>& Controller::dataref()
{
  return m_data;
}

QQmlListProperty<QObject> Controller::data()
{
  return QQmlListProperty<QObject>(this, this, &append, &count, &at, &clear, &replace, &removelast);
}
