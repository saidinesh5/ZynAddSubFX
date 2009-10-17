/*
  ZynAddSubFX - a software synthesizer

  Ranger.h - Control base class
  Copyright (C) 2009-2009 Mark McCurry
  Author: Harald Hvaal

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

#ifndef ARRAYCONTROL_H
#define ARRAYCONTROL_H

#include "GenControl.h"
#include "../globals.h"

class ArrayControl : public GenControl
{
    public:
        ArrayControl(Node *parent, std::string id);
        REALTYPE* readArray(int *size);

        virtual char getCharValue() const;
        virtual void setValue(char val);
        virtual std::string getString() const;
        virtual void defaults();
};

#endif // ARRAYCONTROL_H
