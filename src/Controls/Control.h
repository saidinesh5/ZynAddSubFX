/*
  ZynAddSubFX - a software synthesizer

  Control.h - Control base class
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
#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <vector>
#include <iostream>
#include "GenControl.h"
#include "EventClasses.h"
#include "../Misc/InjFunction.h"
#include "JobClasses.h"

typedef std::vector<std::string> StringVector;

/**A control for a parameter within the program
 *
 * \todo add some bounds checking for sanity*/
template <class T> //please do not define this to be char as InjFunction should have a difficult time
class Control:public GenControl
{
public:
    Control(Node *parent, std::string id, T defaultval, const InjFunction<char,T> *nfunc,enum controlType type);
    virtual ~Control();

    inline T operator()() const {return value;};//It seems to make sense for a control to just return its value when this is called
    std::string getString() const {return "hm, this should get implemented, but not yet :p";};

    virtual std::string getOption(int index) const;
    virtual int numOptions() const;
    void addOption(std::string option) { options.push_back(option); }

    void handleEvent(Event *ev) {}
    void handleSyncEvent(Event *ev);


    void setValue(const T &val); //async
    inline T getValue() const { return value; } //async

    void setCharValue(char val) { value = (*func)(val); } //async
    char getCharValue() const { return (*func)(value); } //async

    void setValue(char val);     //sync, overridden

    void resetDefault();

    void requestValue();

    virtual bool MIDILearn();

protected:
    StringVector options;

private:
    T value;
    T defaultval;

    /**The transformation function for the control*/
    const InjFunction<char,T> *func;
};

#include "Control.cpp"

#endif

