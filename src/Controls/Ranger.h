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
#ifndef RANGER_H
#define RANGER_H

#include "Control.h"
#include "../Misc/InjFunction.h"
#include "../globals.h"

/**
 * A continuiously ranged control
 * that is safe in a threaded environment
 */
class Ranger:public Control<REALTYPE>
{
    public:
        Ranger(Node *parent,
               std::string id,
               REALTYPE defaultval,
               const InjFunction<char, REALTYPE> *nfunc);
        virtual ~Ranger();

        /**@todo get some toString function in here (it only needs to be one way*/
        std::string getString() const
        {return "Example: 50%";}

        void setChar(char val);
        char getChar() const;

    private:
        /**The transformation function for the control*/
        const InjFunction<char, REALTYPE> *func;
};

#endif

