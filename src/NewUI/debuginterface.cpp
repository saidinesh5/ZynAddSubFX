#include "debuginterface.h"
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QScrollBar>
#include "../Misc/Master.h"

EventReceiver::EventReceiver(Node *node)
    : registeredNode(node)
{
    registeredNode->addRedirection(this);
}

void EventReceiver::handleEvent(Event &event)
{
    //this is the place to make it print actual useful info about the event
    QString info;

    if (event.type() == Event::ChangeEvent) {
        info += "(ChangeEvent) ";
        info += "val:" + QString::number(static_cast<ChangeEvent&>(event).getVal());
    } else if (event.type() == Event::NewValueEvent) {
        info += "(NewValueEvent) ";
    } else {
        info += "(Unknown event type: " + int(event.type()) + QString(")");
    }
    emit newEvent(registeredNode, info);
}

class Tree : public QTreeWidget
{
    public:
        NodeUser *user;

        Tree(QWidget *parent)
            : QTreeWidget(parent)
        {
            setColumnCount(1);

            for (NodeIterator i = Node::getRoot()->getChildren().begin();
                    i != Node::getRoot()->getChildren().end();
                    ++i)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(this);
                item->setText(0, QString::fromStdString((*i)->getId()));
                addChildren(*i, item);
            }

        }

        void addChildren(Node *parent, QTreeWidgetItem *parentItem)
        {

            for (NodeIterator i = parent->getChildren().begin();
                    i != parent->getChildren().end();
                    ++i)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                item->setText(0, QString::fromStdString((*i)->getId()));
                addChildren(*i, item);
            }

        }


};


DebugInterface::DebugInterface(QWidget *parent, Master *master)
    : QDialog(parent),
    master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    tree = new Tree(this);

    layout->addWidget(tree);

    text = new QTextEdit(this);

    layout->addWidget(text);

    connect(this, SIGNAL(newEvent(Node*,QString)),
            this, SLOT(receiveEvent(Node*,QString)));

    createEventReceivers(Node::getRoot());

}

void DebugInterface::createEventReceivers(class Node *parent)
{
    for (NodeIterator i = parent->getChildren().begin();
            i != parent->getChildren().end();
            ++i)
    {
        EventReceiver *receiver = new EventReceiver(*i);
        connect(receiver, SIGNAL(newEvent(Node*,QString)),
                this, SIGNAL(newEvent(Node*,QString)));
        createEventReceivers(*i);
    }

}

void DebugInterface::receiveEvent(Node* node, QString info)
{
    static int counter = 0;
    counter++;
    text->append(QString::number(counter) + "] " + QString::fromStdString(node->getAbsoluteId()) + ": " + info);
    text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
}


#include "debuginterface.moc"
// vim: sw=4 sts=4 et tw=100
