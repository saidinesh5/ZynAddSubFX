/*
  ZynAddSubFX - a software synthesizer

  ArrayControlHelper.cpp - A ControlHelper variant for arrays

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

#include "ArrayControlHelper.h"

ArrayControlHelper::ArrayControlHelper(QObject *parent)
    : ControlHelper(parent)
{

}

void ArrayControlHelper::connectedEvent()
{
    ControlHelper::connectedEvent();

    if (ArrayControl* c = static_cast<ArrayControl*>(m_control)) {
        emit arrayUpdated(c);
    }
}

void ArrayControlHelper::disconnectedEvent()
{
}

void ArrayControlHelper::newValueEvent()
{
    ControlHelper::newValueEvent();

    if (ArrayControl* c = static_cast<ArrayControl*>(m_control)) {
        emit arrayUpdated(c);
    }
}

#include "ArrayControlHelper.moc"
// vim: sw=4 sts=4 et tw=100
