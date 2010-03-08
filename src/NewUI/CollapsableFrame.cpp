/*
  ZynAddSubFX - a software synthesizer

  CollapsableFrame.cpp - A QFrame that can be collapsed by a button

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

#include "CollapsableFrame.h"
#include "ControlHelper.h"

CollapsableFrame::CollapsableFrame(QWidget *parent, QWidget *expanded)
    :QFrame(parent),
      expanded(expanded)
{
    setupUi(this);
    connect(collapseButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsed()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteMe()));

    layout()->addWidget(expanded);
    QList<QWidget *> childs = expanded->findChildren<QWidget *>();
    foreach(QWidget * child, childs)
    {
        if(child->property("showOnExpand").toBool()) {
            showOnExpand.append(child);
            child->hide();
        }
        if(child->property("hideOnExpand").toBool()) {
            hideOnExpand.append(child);
            child->show();
        }
    }
}

bool CollapsableFrame::isCollapsed()
{
    return (collapseButton->text() == ">");
}

void CollapsableFrame::toggleCollapsed()
{
    if(!expanded)
        return;

    if(isCollapsed()) {
        collapseButton->setText("\\/");

        foreach(QWidget * child, showOnExpand)
        {
            child->show();
        }
        foreach(QWidget * child, hideOnExpand)
        {
            child->hide();
        }
    }
    else {
        collapseButton->setText(">");
        foreach(QWidget * child, showOnExpand)
        {
            child->hide();
        }
        foreach(QWidget * child, hideOnExpand)
        {
            child->show();
        }
    }
}

void CollapsableFrame::deleteMe()
{
    if(!expanded)
        return;

    QString id = ControlHelper::findComponentPath(expanded);
    if(id.isEmpty())
        return;

    Node::lock();
    Node *node = Node::get(id.toStdString());
    if(!node)
        return;

    node->removeFromParent();
    Node::unlock();
}

void CollapsableFrame::on_enabledCheck_stateChanged(int state)
{
    if(!expanded)
        return;

    if(state == Qt::Checked)
        expanded->setEnabled(true);
    else
    if(state == Qt::Unchecked)
        expanded->setEnabled(false);
}

#include "CollapsableFrame.moc"
// vim: sw=4 sts=4 et tw=100

