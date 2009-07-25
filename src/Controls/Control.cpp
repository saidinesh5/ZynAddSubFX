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

template <class T>
Control<T>::Control(Node *parent, std::string id, T defaultval, const InjFunction<char,T> *nfunc,enum controlType type)
    :GenControl(parent,id,type),value(defaultval),defaultval(defaultval),func(nfunc)
{
    for (char c = 0; c < 127; ++c) {
        T v = (*func)(c);
        char backval = char((*func)(v));
        if (backval != c) {
            std::cerr << " Warning: (" << int(c) << " != " << int(backval) << ") function is not reflective\n";
            break;
        }
    }
}

template <class T>
Control<T>::~Control()
{
    delete func;
}

template <class T>
void Control<T>::handleSyncEvent(Event *ev)
{
    //this is when the control value is to be changed to
    //something else
    if (ev->type() == Event::ChangeEvent) {
        char charval = static_cast<ChangeEvent*>(ev)->getVal();
        value = (*func)(charval);
        std::cout << getAbsoluteId() << "Received charval: " << int(charval) << " and new value " << value << "\n";
        forward(new NewValueEvent(charval));

        //and this is for reading the value of the control
    } else if (ev->type() == Event::RequestValueEvent) {
        forward(new NewValueEvent((*func)(value)));
    }
}

template <class T>
void Control<T>::setValue(const T &val)
{
    value = val;
    forward(new NewValueEvent((*func)(value)));
}

template <class T>
void Control<T>::setValue(char val)
{
    Job::push(new NodeJob(*this, new ChangeEvent(val)));
}

template <class T>
void Control<T>::requestValue()
{
    Job::push(new NodeJob(*this, new RequestValueEvent()));
}

template <class T>
void Control<T>::resetDefault()
{
    setValue(defaultval);
}
