#ifndef EVENTHELPER_H
#define EVENTHELPER_H

#include "../Controls/Node.h"
#include <QObject>

class EventHelper:public QObject, public NodeUser
{
    Q_OBJECT

    public:
        EventHelper(QObject *parent);
        void handleEvent(Event *event);
        static class EventHelper *getInstance();

    signals:
        //void childAdded

    private:
        static class EventHelper * theInstance;
};

#endif // EVENTHELPER_H

