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
#ifndef TOGGLE_H
#define TOGGLE_H

#include <string>
#include "Control.h"

/**
 * A representation of a bistate switch
 * AKA an on/off switch
 */
class Toggle:public Control<bool>
{
    public:
        Toggle(Node *parent, std::string id, bool defaultval);
        ~Toggle();

        std::string getString() const;
        void setInt(int val);
        int getInt() const;

        inline void operator=(bool b) {
            setValue(b);
        }

        virtual void addpar(class XMLwrapper *xml, const std::string& name);
        virtual void getpar(class XMLwrapper *xml, const std::string& name);
};

#endif

