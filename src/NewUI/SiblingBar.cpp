/*
  ZynAddSubFX - a software synthesizer

  SiblingBar.cpp - A tabbar that can be used for controls having child controls

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

#include "SiblingBar.h"
#include <QEvent>
#include "../Controls/Node.h"
#include "ControlHelper.h"
#include <QSet>
#include <QtDebug>
#include <QMouseEvent>
#include <QMenu>

SiblingBar::SiblingBar(QWidget *parent)
    :QTabBar(parent),
      m_childContainer(NULL),
      m_controlsWidget(NULL)
{
    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(updateFoundControls()));
}

void SiblingBar::setControlsWidget(QWidget *widget)
{
    m_controlsWidget = widget;
}

void SiblingBar::setNode(Node *container)
{
    if(!container) {
        qDebug() << "NULL-container, returning";
        return;
    }

    NodeIterator it = container->getChildren().begin();
    while(it != container->getChildren().end()) {
        addTab(QString::fromStdString((*it)->getId()));
        it++;
    }

    m_childContainer = container;

    updateFoundControls();
}

void SiblingBar::updateFoundControls()
{
    qDebug() << "updateFoundControls";

    if(!m_childContainer || !m_controlsWidget)
        return;

    QString childAbsoluteId;

    if(m_childContainer->getChildren().size())
        childAbsoluteId = QString::fromStdString(
            m_childContainer->getChildren().at(currentIndex())->getAbsoluteId());
    else
        childAbsoluteId = QString();
    m_controlsWidget->setProperty("absoluteControlId", childAbsoluteId);
}

void SiblingBar::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() != Qt::RightButton))
        return;
    if(!m_childContainer)
        return;

    int tabIndex = -1;
    for(int i = 0; i < count(); ++i)
        if(tabRect(i).contains(event->pos())) {
            tabIndex = i;
            break;
        }


    QMenu menu(this);

    QList<QAction *> typeActions;
    for(unsigned int i = 0; i < m_childContainer->getTypes().size(); ++i)
        typeActions.append(menu.addAction("&Add new "
                                          + QString::fromStdString(
                                              m_childContainer->getTypes().at(i))));

    QAction *duplicate = NULL, *deleteAction = NULL;
    if(tabIndex != -1)
        duplicate = menu.addAction("&Duplicate");
    if(tabIndex != -1)
        deleteAction = menu.addAction("&Delete");

    QAction *response = menu.exec(event->globalPos());

    if(response == duplicate) {}
    else
    if(response == deleteAction) {
        QString tabname = tabText(tabIndex);
        removeTab(tabIndex);
        updateFoundControls();
        m_childContainer->removeChild(tabname.toStdString());
        return;
    }

    int typeIndex = typeActions.indexOf(response);
    if(typeIndex != -1) {
        std::string stdChild = m_childContainer->createChild(typeIndex);
        QString     childId  = QString::fromStdString(stdChild);
        if(!childId.isEmpty()) {
            Node::lock();
            Node *newContainer = Node::get(stdChild);
            addTab(QString::fromStdString(newContainer->getId()));
            Node::unlock();
        }
        qDebug() << "Created child at " << childId;
    }
}

QString SiblingBar::getCurrentChild()
{
    if(!m_childContainer)
        return QString();
    if(!m_childContainer->getChildren().size())
        return QString();

    return QString::fromStdString(m_childContainer->getChildren().at(
                                      currentIndex())->getAbsoluteId());
}

#include "SiblingBar.moc"

