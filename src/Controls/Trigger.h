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
#ifndef TRIGGER_H
#define TRIGGER_H

#include "GenControl.h"

/**
 * A stateless trigger
 * Ex: the Panic Button
 */
class Trigger:public GenControl
{
    public:
        Trigger(Node *parent, std::string id, std::string nlabel);
        virtual ~Trigger();

        std::string getString() const
        {return label;}

        void setValue(char val);

    private:
        /**The transformation function for the control*/
        const std::string label;
};

#endif

