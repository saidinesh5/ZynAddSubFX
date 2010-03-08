/*
  ZynAddSubFX - a software synthesizer

  Button.cpp - A pushbutton

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

#include "Button.h"
#include "ControlHelper.h"
#include <QtDebug>

Button::Button(QWidget *parent)
    : QPushButton(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(clicked()),
            helper, SLOT(trigger()));
}


#include "Button.moc"
// vim: sw=4 sts=4 et tw=100
