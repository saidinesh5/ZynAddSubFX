/*
  ZynAddSubFX - a software synthesizer

  CollapsableFrame.h - A QFrame that can be collapsed by a button

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

#ifndef COLLAPSABLEFRAME_H
#define COLLAPSABLEFRAME_H

#include <QFrame>
#include "ui_CollapsableFrame.h"

class CollapsableFrame:public QFrame, public Ui::CollapsableFrame
{
    Q_OBJECT

    public:
        CollapsableFrame(QWidget *parent, QWidget *expanded);
        void setExpandedWidget(QWidget *widget);
        void setCollapsedWidget(QWidget *widget);

    public slots:
        void toggleCollapsed();
        void deleteMe();
        void on_enabledCheck_stateChanged(int state);

    private:
        bool isCollapsed();

        QWidget *expanded;
        QList<QWidget *> showOnExpand;
        QList<QWidget *> hideOnExpand;
};

#endif // COLLAPSABLEFRAME_H

