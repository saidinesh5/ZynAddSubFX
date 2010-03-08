/*
  ZynAddSubFX - a software synthesizer

  DebugInterface.h - A debug interface for developing the ZynAddSubFX Qt GUI

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

#ifndef DEBUGINTERFACE_H
#define DEBUGINTERFACE_H

#include <QDialog>
#include "../Controls/Node.h"
#include <QList>
#include<QTreeWidgetItem>

class EventReceiver:public QObject, public NodeUser
{
    Q_OBJECT
    public:
        EventReceiver(Node *node);
        virtual ~EventReceiver();
        void handleEvent(Event *event);
        Node *registeredNode;

        QString absoluteId;

    signals:
        void newEvent(Node *node, QString info);
};

class DebugInterface:public QDialog
{
    Q_OBJECT

    public:
        DebugInterface(QWidget *parent = NULL, class Master *master = NULL);
        void handleEvent(Event *event);

    signals:
        void newEvent(Node *node, QString info);

    private slots:
        void receiveEvent(Node *node, QString info);
        void refreshTree();
        void createEventReceivers(class Node * parent);

    private:

        QList<EventReceiver *> receivers;
        class Master * master;
        class QTextEdit * text;
        class QLineEdit * filter;
};

#endif // _DEBUGINTERFACE_H

