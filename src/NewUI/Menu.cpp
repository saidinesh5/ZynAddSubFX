/*
  ZynAddSubFX - a software synthesizer

  Menu.cpp - A menu used for all widgets in the ui

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

#include <QMenu>
#include "Menu.h"
#include <QtDebug>

Menu::Menu(QWidget *parent, ControlHelper *helper)
    : QObject(parent),
    m_helper(helper)
{
    parent->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(parent, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(menuRequested(const QPoint&)));
}

void Menu::menuRequested(const QPoint& pos)
{
    QAction *defaults = new QAction("Default value", NULL);
    QAction *midiLearn = new QAction("MIDI Learn", NULL);
    QAction *debugPrint = new QAction("(Debug print)", NULL);

    QList<QAction*> actions;
    actions << defaults << midiLearn << debugPrint;

    QWidget *widget = qobject_cast<QWidget*>(parent());
    if (!widget) return;

    QAction *response = QMenu::exec(actions, widget->mapToGlobal(pos));
    if (midiLearn == response) {
        m_helper->MIDILearn();
    } else if (defaults == response) {
        m_helper->defaults();
    } else if (debugPrint == response) {
        m_helper->debugPrint();
    }
}

#include "Menu.moc"

// vim: sw=4 sts=4 et tw=100
