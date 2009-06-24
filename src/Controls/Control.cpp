/*
  ZynAddSubFX - a software synthesizer

  Control.C - Control base class
  Copyright (C) 2009-2009 Mark McCurry
  Author: Mark McCurry

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
#include "Control.h"

Control::Control(ControlContainer *parent, std::string id, std::string description, char ndefaultval)
        :defaultval(ndefaultval),
        lockqueue(-1),
        locked(false),
        m_id(id),
        m_description(description),
        m_parent(parent)
{
    parent->addControl(this);
}

void Control::lock()
{
    locked=true;
    lockqueue=-1;
}

void Control::ulock()
{
    if (locked&&lockqueue>=0)
        setValue(lockqueue);
    locked=false;
}

void Control::registerUser(class ControlUser *user)
{
    m_users.push_back(user);
}

const std::string& Control::getId()
{
    return m_id;
}

const std::string Control::getAbsoluteId()
{
    if (!m_parent) return getId();
    return m_parent->getAbsoluteId() + "." + m_id;

}

