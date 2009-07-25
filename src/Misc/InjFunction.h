/*
  ZynAddSubFX - a software synthesizer

  InjFunction.h - Functions Utility Classes
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
#ifndef _INJFUNCTION_H
#define _INJFUNCTION_H

#include "../globals.h"

/**Injective Function
 *
 * This means that the object contains a function that will translate
 * from type A to type B using a one-to-one function and its inverse
 * Note: type A cannot be equal to type B */

template <class A,class B>
class InjFunction
{
public:
    virtual B operator()(const A &x)const = 0;
    virtual A operator()(const B &x)const = 0;

    char round(REALTYPE val) {
        return char(val + 0.5);
    }
};
#endif

