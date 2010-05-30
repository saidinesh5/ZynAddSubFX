/*
  ZynAddSubFX - a software synthesizer

  Function.h - Functions Utility Classes
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
#ifndef _LININJFUNC_H
#define _LININJFUNC_H
#include "InjFunction.h"

template<class T>
class LinInjFunc:public InjFunction<int, T>
{
    public:
        LinInjFunc(T min, T max)
            :b(min), m((max - min) / 127) {}
        inline T operator()(const int &x) const {return m * x + b; }
        inline int operator()(const T &x) const {return (int)((x - b) / m); }
    private:
        T b, m;
};

#endif

