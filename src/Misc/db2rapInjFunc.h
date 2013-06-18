/*
  ZynAddSubFX - a software synthesizer

  db2rapInjFunc.h - Functions Utility Classes
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
#ifndef _DB2RAPINJFUNC_H
#define _DB2RAPINJFUNC_H
#include "InjFunction.h"
#include "../globals.h"
#include <iostream>
#include <cmath>
template<class T>
class db2rapInjFunc:public InjFunction<int, T>
{
    public:
        db2rapInjFunc(T min, T max)
            :b(min), m((max - min)) {}

        inline T operator()(const int &x) const {return dB2rap(b + m * x / 127); }
        inline int operator()(const T &x) const {return round(127.0
                                                               * (rap2dB(
                                                                      x)
                                                                  - b) / m);  }
    private:
        T b, m;
};

#endif

