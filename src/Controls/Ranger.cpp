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
#include "JobClasses.h"
#include "Ranger.h"

using namespace std;

Ranger::Ranger(Node *parent, std::string id, REALTYPE defaultval, const InjFunction<char,REALTYPE> *nfunc)
    :Control<REALTYPE>(parent,id,defaultval),func(nfunc)
{
    if(func==NULL)
        cerr << "THIS SHOULD NEVER HAPPEN!" << __FILE__ << __LINE__ << endl;

    //test func
    for (char c = 0; c < 127; ++c) {
        REALTYPE v = (*func)(c);
        char backval = char((*func)(v));
        if (backval != c) {
            cerr << " Warning (Control.cpp:" << __LINE__ << "): (" << int(c) << " != " << int(backval) << ") function is not reflective\n";
            break;
        }
    }
}

Ranger::~Ranger()
{
    delete func;
}

void Ranger::setCharValue(char val)
{
    setValue(val);
} 

char Ranger::getCharValue() const
{ 
    return (*func)(getValue());
} 

void Ranger::setValue(char val)
{
    Control<REALTYPE>::setValue((REALTYPE)(*func)((char)val));
}

