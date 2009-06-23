#include "eventhelper.h"
#include <QEvent>
#include "../Controls/ControlContainer.h"
#include <QtDebug>

EventHelper* EventHelper::theInstance = NULL;

EventHelper::EventHelper(QObject *parent)
        : QObject(parent)
{
    qDebug() << "Registering at event";
    Event::registerUser(this);
}

bool EventHelper::eventFilter(Event *event)
{
    qDebug() << event;
}

EventHelper* EventHelper::getInstance()
{
    if (!theInstance) {
        theInstance = new EventHelper(NULL);
    }
}

#include "eventhelper.moc"
// vim: sw=4 sts=4 et tw=100
