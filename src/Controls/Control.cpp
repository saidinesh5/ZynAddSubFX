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
Control<T>::Control(Node *parent, std::string id, T defaultval)
    :GenControl(parent,id),value(defaultval),defaultval(defaultval)
{}

template <class T>
Control<T>::~Control()
{}

template <class T>
T Control<T>::operator()() const
{
    return this->getValue();
}

template <class T>
void Control<T>::handleEvent(Event *ev)
{
    //this is when the control value is to be changed to
    //something else
    if (ev->type() == Event::ChangeEvent) {
        char charval = static_cast<ChangeEvent*>(ev)->getVal();
        setValue(charval);
        forward(new NewValueEvent(charval, this));

        //and this is for reading the value of the control
    } else if (ev->type() == Event::RequestValueEvent) {
        forward(new NewValueEvent(getCharValue(), this));
    }
}

template <class T>
void Control<T>::setValue(const T &val)
{
    bool changed = false;
    pthread_mutex_lock(&localMute);
    if(value!=val){
        value = val;
        changed = true;
    }
    pthread_mutex_unlock(&localMute);
    if(true)
        std::cout << "Setting to " << val << std::endl;
    if(changed)
        forward(new NewValueEvent(getCharValue(), this));
}

template <class T>
T Control<T>::getValue() const
{
    T tmp; 
    pthread_mutex_lock(&localMute);
    tmp = value;
    pthread_mutex_unlock(&localMute);
    return tmp;
}

template <class T>
void Control<T>::setCharValue(char val)
{
    setValue(T(val));
} 

template <class T>
char Control<T>::getCharValue() const
{ 
    return (char)getValue();
}

template <class T>
void Control<T>::setValue(char val)
{
    setValue(T(val));
}

template <class T>
void Control<T>::defaults()
{
    setValue(defaultval);
}

