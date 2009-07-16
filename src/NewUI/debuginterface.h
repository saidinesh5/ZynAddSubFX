#ifndef DEBUGINTERFACE_H
#define DEBUGINTERFACE_H

#include <QDialog>
#include "../Controls/Node.h"
#include <QList>

class EventReceiver : public QObject, public NodeUser
{
    Q_OBJECT
    public:
        EventReceiver(Node *node);
        virtual ~EventReceiver();
        void handleEvent(Event *event);
        Node *registeredNode;

    signals:
        void newEvent(Node *node, QString info);
};

class DebugInterface : public QDialog
{
    Q_OBJECT

    public:
        DebugInterface(QWidget *parent = NULL, class Master *master = NULL);
        void handleEvent(Event *event);

    signals:
        void newEvent(Node *node, QString info);

    private slots:
        void receiveEvent(Node* node, QString info);
        void refreshTree();
        void createEventReceivers(class Node *parent);

    private:

        QList<EventReceiver*> receivers;
        class Master *master;
        class QTextEdit *text;
        class Tree *tree;

};

#endif // _DEBUGINTERFACE_H
