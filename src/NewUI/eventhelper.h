#ifndef EVENTHELPER_H
#define EVENTHELPER_H

#include "../Controls/Event.h"
#include <QObject>

class EventHelper : public QObject, public EventUser
{
    Q_OBJECT

public:
    EventHelper(QObject *parent);
    bool eventFilter(Event *event);
    static class EventHelper* getInstance();

signals:
    //void childAdded

private:
    static class EventHelper *theInstance;

};

#endif // EVENTHELPER_H
