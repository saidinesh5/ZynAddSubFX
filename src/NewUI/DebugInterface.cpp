/*
  ZynAddSubFX - a software synthesizer

  DebugInterface.cpp - A debug interface for developing the ZynAddSubFX Qt GUI

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "DebugInterface.h"
#include <QLineEdit>
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

/**
 * The debuginterface provides some simple tools usable for debugging the application, and as such
 * not much time is really spent on making the ui userfriendly.
 * Disclaimer: Because it is also open to be changed rapidly for the developer's convenience, I
 * (harald) haven't spent very much making it modular and pretty, mostly just focusing on getting
 * the information I need.
 */


static class Tree * tree;

EventReceiver::EventReceiver(Node *node)
    :registeredNode(node)
{
    registeredNode->addRedirection(this);
    absoluteId = QString::fromStdString(registeredNode->getAbsoluteId());
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

    info += "<" + absoluteId + "> ";

    if(event->type() == Event::ChangeEvent) {
        info += "(ChangeEvent) ";
        info += "val:" + QString::number(
            static_cast<ChangeEvent *>(event)->val);
    }
    else
    if(event->type() == Event::NewValueEvent) {

        NewValueEvent* ev = static_cast<NewValueEvent *>(event);

        info += "(NewValueEvent) ";
        info += "val: " + QString::number(
            ev->value);

        if (const Control<int>* c = dynamic_cast<const Control<int>*>(ev->control)) {
            info += ", " + QString::number( c->getValue());
        }

        if (const Control<float>* c = dynamic_cast<const Control<float>*>(ev->control)) {
            info += ", " + QString::number( c->getValue());
        }

    }
    else
    if(event->type() == Event::RemovalEvent) {
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
        info += "(Unknown event type: " + QString::number(int(event->type())) + QString(")");

    emit newEvent(registeredNode, info);
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
            for(unsigned int j = 0; j < (node)->m_rules.size(); ++j) {
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

            //expandAll();
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

            qDebug() << "Value is " << slider->value();

            setIndexWidget(indexFromItem(item, 1), slider);
        }

};


DebugInterface::DebugInterface(QWidget *parent, Master *master)
    :QDialog(parent),
      master(master)
{

    setWindowTitle("Debug Interface");

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    tree = new Tree(this);

    layout->addWidget(tree);


    QPushButton *button = new QPushButton("Refresh tree");
    layout->addWidget(button);

    connect(button, SIGNAL(clicked()),
            this, SLOT(refreshTree()));

    text = new QTextEdit(this);

    filter = new QLineEdit(this);

    layout->addWidget(filter);

    layout->addWidget(text);

    layout->setStretchFactor(text, 3);
    layout->setStretchFactor(tree, 9);
    layout->setStretchFactor(filter, 1);

    connect(this, SIGNAL(newEvent(Node *, QString)),
            this, SLOT(receiveEvent(Node *, QString)));

    createEventReceivers(Node::getRoot());
    EventReceiver *receiver = new EventReceiver(Node::getRoot());
    receivers.append(receiver);
    connect(receiver, SIGNAL(newEvent(Node *, QString)),
            this, SIGNAL(newEvent(Node *, QString)));

    resize(800, 600);
}

void DebugInterface::createEventReceivers(class Node *parent)
{
    for(NodeIterator i = parent->getChildren().begin();
        i != parent->getChildren().end();
        ++i) {
        EventReceiver *receiver = new EventReceiver(*i);
        receivers.append(receiver);
        connect(receiver, SIGNAL(newEvent(Node *, QString)),
                this, SIGNAL(newEvent(Node *, QString)));
        createEventReceivers(*i);
    }
}

bool containWords(QString terms, QString data)
{
    QStringList t = terms.split(" ");
    foreach (const QString& term, terms) {
        if (!data.contains(term)) {
            return false;
        }
    }
    return true;
}

void DebugInterface::receiveEvent(Node * /*node */, QString info)
{
    static int counter = 0;
    counter++;

    QString msg;

    msg += QString::number(counter) + "] ";

    msg += info;

    if (filter->text().isEmpty() || containWords(filter->text(), msg))
        text->append(msg);

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

