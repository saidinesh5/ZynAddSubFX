#include "DebugInterface.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QScrollBar>
#include "../Misc/Master.h"
#include <QtDebug>
#include "Slider.h"


static class Tree * tree;

EventReceiver::EventReceiver(Node *node)
    :registeredNode(node)
{
    registeredNode->addRedirection(this);
}

EventReceiver::~EventReceiver()
{
    if(registeredNode)
        registeredNode->removeRedirections(this);
}

void EventReceiver::handleEvent(Event *event)
{
    //this is the place to make it print actual useful info about the event
    QString info;
    bool    safeprint = false;

    if(event->type() == Event::ChangeEvent) {
        info += "(ChangeEvent) ";
        info += "val:" + QString::number(
            static_cast<ChangeEvent *>(event)->getVal());
    }
    else
    if(event->type() == Event::NewValueEvent)
        info += "(NewValueEvent) ";
    else
    if(event->type() == Event::RemovalEvent) {
        RemovalEvent *rem = static_cast<RemovalEvent *>(event);
        safeprint      = true;
        info          += "(RemovalEvent) ";
        registeredNode = NULL;
    }
    else
    if(event->type() == Event::MidiEvent) {
        MidiEvent *mid = static_cast<MidiEvent *>(event);
        info += "(MidiEvent) ";
        info += QString::number(mid->chan) + ", ";
        info += QString::number(mid->type) + ", ";
        info += QString::number(mid->par);
    }
    else
        info += "(Unknown event type: " + int(event->type()) + QString(")");
    emit newEvent(registeredNode, info, safeprint);
}

class Tree:public QTreeWidget
{
    public:
        NodeUser *user;

        Tree(QWidget *parent)
            :QTreeWidget(parent)
        {
            refresh();
        }

        QString makeConnections(Node *node)
        {
            QString connections;
            for(int j = 0; j < (node)->m_rules.size(); ++j) {
                NodeUser *user = (node)->m_rules.at(j).destination;

                //we don't need the dynamic ones from this app
                if(dynamic_cast<EventReceiver *>(user))
                    continue;

                Node *node = dynamic_cast<Node *>(user);
                if(node)
                    connections +=
                        QString::fromStdString(node->getAbsoluteId()) + ", ";
                else
                    connections += QString::number((long int)(user)) + ", ";
            }
            return connections;
        }

        void refresh()
        {
            clear();
            setColumnCount(2);

            for(NodeIterator i = Node::getRoot()->getChildren().begin();
                i != Node::getRoot()->getChildren().end();
                ++i) {
                QTreeWidgetItem *item = new QTreeWidgetItem(this);
                item->setText(0, QString::fromStdString((*i)->getId()));
                item->setText(1, makeConnections(*i));
                item->setToolTip(0, QString::fromStdString((*i)->getAbsoluteId()));
                addChildren(*i, item);
            }

            expandAll();
            resizeColumnToContents(0);
        }

        void addChildren(Node *parent, QTreeWidgetItem *parentItem)
        {
            for(NodeIterator i = parent->getChildren().begin();
                i != parent->getChildren().end();
                ++i) {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                item->setText(0, QString::fromStdString((*i)->getId()));
                item->setText(1, makeConnections(*i));
                item->setToolTip(0, QString::fromStdString((*i)->getAbsoluteId()));
                addChildren(*i, item);
            }
        }

        void mouseDoubleClickEvent(QMouseEvent *event)
        {

            QTreeWidgetItem *item = itemAt(event->pos());

            if (!item) return;

            Slider *slider = new Slider(NULL);
            slider->show();
            slider->setMinimum(0);
            slider->setMaximum(127);
            slider->setOrientation(Qt::Horizontal);
            slider->setProperty("absoluteControlId", item->toolTip(0));
            setIndexWidget(indexFromItem(item, 1), slider);
        }

};


DebugInterface::DebugInterface(QWidget *parent, Master *master)
    :QDialog(parent),
      master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    tree = new Tree(this);

    layout->addWidget(tree);


    QPushButton *button = new QPushButton("Refresh tree");
    layout->addWidget(button);

    connect(button, SIGNAL(clicked()),
            this, SLOT(refreshTree()));

    text = new QTextEdit(this);

    layout->addWidget(text);

    layout->setStretchFactor(text, 1);
    layout->setStretchFactor(tree, 3);

    connect(this, SIGNAL(newEvent(Node *, QString, bool)),
            this, SLOT(receiveEvent(Node *, QString, bool)));

    createEventReceivers(Node::getRoot());
    EventReceiver *receiver = new EventReceiver(Node::getRoot());
    receivers.append(receiver);
    connect(receiver, SIGNAL(newEvent(Node *, QString, bool)),
            this, SIGNAL(newEvent(Node *, QString, bool)));

    resize(800, 600);
}

void DebugInterface::createEventReceivers(class Node *parent)
{
    for(NodeIterator i = parent->getChildren().begin();
        i != parent->getChildren().end();
        ++i) {
        EventReceiver *receiver = new EventReceiver(*i);
        receivers.append(receiver);
        connect(receiver, SIGNAL(newEvent(Node *, QString, bool)),
                this, SIGNAL(newEvent(Node *, QString, bool)));
        createEventReceivers(*i);
    }
}

void DebugInterface::receiveEvent(Node *node, QString info, bool safeprint)
{
    static int counter = 0;
    counter++;
    text->append(QString::number(counter) + "] ");
    if(!safeprint)
        text->append(QString::fromStdString(node->getAbsoluteId()) + ": ");
    text->append(info);
    text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
}

void DebugInterface::refreshTree()
{
    tree->refresh();
    foreach(EventReceiver * receiver, receivers)
    {
        delete receiver;
    }
    receivers.clear();
    createEventReceivers(Node::getRoot());
}


#include "DebugInterface.moc"
// vim: sw=4 sts=4 et tw=100

