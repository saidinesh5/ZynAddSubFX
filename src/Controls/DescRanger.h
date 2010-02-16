/*
  ZynAddSubFX - a software synthesizer

  Ranger.h - Control base class
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
#ifndef DESCRANGER_H
#define DESCRANGER_H

#include "Control.h"
#include "../Misc/InjFunction.h"
#include "../globals.h"

/**
 * A Descretly ranged control
 * that is safe in a threaded environment
 */
class DescRanger:public Control<int>
{
    public:
        DescRanger(Node *parent, std::string id, int defaultval, int min  = 0, int max = 127);
        virtual ~DescRanger();

        /**@todo get some toString function in here (it only needs to be one way*/
        std::string getString() const
        {return "Example: 50%";}

        virtual void addpar(class XMLwrapper *xml, const std::string& name);
        virtual void getpar(class XMLwrapper *xml, const std::string& name);

};

#endif

