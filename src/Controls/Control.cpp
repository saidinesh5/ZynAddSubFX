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

template<class T>
Control<T>::Control(Node *parent, std::string id, T defaultval)
    :GenControl(parent, id), value(defaultval), defaultval(defaultval),
    min(0),
    max(127)
{}

template<class T>
Control<T>::~Control()
{}

template<class T>
T Control<T>::operator()() const
{
    return this->getValue();
}

template<class T>
void Control<T>::setValue(const T &val)
{
    bool changed = false;
    lock();
    if(value != val) {
        value   = val;
        changed = true;
    }
    unlock();
    //std::cout << "Setting to " << val << " " << (long long)(this) << std::endl;
    if(changed)
        forward(new NewValueEvent(this));
}

template<class T>
T Control<T>::getValue() const
{
    T tmp;
    lock();
    tmp = value;
    unlock();
    return tmp;
}

template<class T>
void Control<T>::defaults()
{
    setValue(defaultval);
}

template<class T>
void Control<T>::setDefaults( const T &val)
{
    defaultval = val;
}

template<class T>
void Control<T>::storeDefaults()
{
    setDefaults(value);
}

template<class T>
void Control<T>::setChar(char val)
{
    setValue(T(val));
}

template<class T>
char Control<T>::getChar() const
{
    T v = getValue();
    if (v < 0 || v > 127) {
        std::cout << "getChar(): Warning, value truncated\n";
    }
    return char(v);
}

template<class T>
void Control<T>::addpar(XMLwrapper *xml, const std::string& name)
{
    xml->addpar(name, getchar());
}

template<class T>
void Control<T>::getpar(XMLwrapper *xml, const std::string& name)
{
    setChar(xml->getpar127(name, getChar()));
}


