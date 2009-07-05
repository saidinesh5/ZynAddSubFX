#include "eventhelper.h"
#include <QEvent>
#include "../Controls/Node.h"
#include <QtDebug>

EventHelper* EventHelper::theInstance = NULL;

EventHelper::EventHelper(QObject *parent)
        : QObject(parent)
{
}

void EventHelper::handleEvent(Event &event)
{
}

EventHelper* EventHelper::getInstance()
{
    if (!theInstance) {
        theInstance = new EventHelper(NULL);
    }
}

#include "eventhelper.moc"
// vim: sw=4 sts=4 et tw=100
