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


/*
 * The two macros REALINJFUNC and REALINJFUNCFUNC are convenience macros for making injfunction
 * classes to be used locally in the code. REALINJFUNC will define a class that does the REAL/char
 * conversion, and REALINJFUNCFUNC will in addition define two functions that can be used for
 * performing the conversion without making an instance of the injfunction class. This is useful for
 * example when you need to perform such a conversion during the initialization of the controls in
 * the class constructor.
 *
 * Here's an example, taken from FilterParams.cpp:
 *
 * REALINJFUNCFUNC(FreqInj,
 *              freq2char, freq2real,
 *              ((x / 5.0) + 1.0) * 64.0,
 *              (x / 64.0 - 1.0) * 5.0
 *             );
*
* Here FreqInj is the name of the class, freq2char is the function that will do REAL->char
* conversion, freq2real is the vice versa. The next two inputs are the actual conversions that will
* be done, with the parameter 'x' as the input.
*
* NOTE: These two conversions should be the inverse of each other! ie, if you convert a char to a
* real and back, you should have the same number.
*
* TIP: use the round function for mathematical rounding
*/

#define REALINJFUNC(NAME, TOCHAR, TOREAL) \
    class NAME:public InjFunction < char, REALTYPE > { \
        public: inline char operator()(const REALTYPE &x) const \
            {return round(TOCHAR);} \
            inline REALTYPE operator()(const char &x) const \
            {return (TOREAL);} };

#define REALINJFUNCFUNC(NAME, TOCHARNAME, TOREALNAME, TOCHAR, TOREAL) \
    REALINJFUNC(NAME, TOCHAR, TOREAL); \
    inline char TOCHARNAME(const REALTYPE & x) \
    { return round(TOCHAR); } \
    inline REALTYPE TOREALNAME(const char &x) \
    { return (TOREAL); }


/**Injective Function
 *
 * This means that the object contains a function that will translate
 * from type A to type B using a one-to-one function and its inverse
 * Note: type A cannot be equal to type B */

template<class A, class B>
class InjFunction
{
    public:
        virtual B operator()(const A &x) const = 0;
        virtual A operator()(const B &x) const = 0;

        char round(REALTYPE val) const {
            return char(val + 0.5);
        }
};
#endif

