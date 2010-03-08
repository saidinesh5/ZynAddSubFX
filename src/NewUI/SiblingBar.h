/*
  ZynAddSubFX - a software synthesizer

  SiblingBar.h - A tabbar that can be used for controls having child controls

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

#ifndef _SIBLINGBAR_H_
#define _SIBLINGBAR_H_

#include <QTabBar>
#include <QSet>

class SiblingBar:public QTabBar
{
    Q_OBJECT

    public:
        SiblingBar(QWidget *parent = NULL);
        void setControlsWidget(QWidget *widget);
        void setNode(class Node * container);
        void mouseReleaseEvent(class QMouseEvent * event);
        QString getCurrentChild();

    private slots:
        void updateFoundControls();

    private:
        QSet<class ControlHelper *> m_foundControls;
        class Node * m_childContainer;
        QWidget *m_controlsWidget;
};

#endif /* ifndef _SIBLINGBAR_H_ */

