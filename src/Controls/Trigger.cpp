/*
  ZynAddSubFX - a software synthesizer

  Control.cpp - Control template
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
#include "Trigger.h"
#include "EventClasses.h"

using namespace std;

Trigger::Trigger(Node *parent, string id, string nlabel)
    :GenControl(parent, id), label(nlabel)
{}

Trigger::~Trigger()
{}

void Trigger::setInt(int val)
{
    if(val >= 64)
        //this will be useful when sources are identifyiable
        //or if there is a more generic event
        forward(new NewValueEvent(this));
}

