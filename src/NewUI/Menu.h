/*
  ZynAddSubFX - a software synthesizer

  Menu.h - A menu used for all widgets in the ui

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

#ifndef MENU_H
#define MENU_H

#include "ControlHelper.h"
#include <QObject>

/** 
 * @brief A menu, used for all widgets that are connected to controls in the gui. Automatically
 * assigned through the ControlHelper class.
 */
class Menu : public QObject
{
    Q_OBJECT
    public:
        Menu(QWidget *parent, ControlHelper* helper);

    private slots:
        void menuRequested(const QPoint& pos);

    private:
        ControlHelper *m_helper;

};

#endif // MENU_H
