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
    :GenControl(parent, id),
    options(0),
    min(0),
    max(127),
    value(defaultval),
    defaultval(defaultval)
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
    if(value != val) {
        value   = val;
        changed = true;
    }
    if(changed)
        forward(new NewValueEvent(this, getInt()));
}

template<class T>
T Control<T>::getValue() const
{
    return value;
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
void Control<T>::setInt(int val)
{
    setValue(T(val));
}

template<class T>
int Control<T>::getInt() const
{
    T v = getValue();
    return int(v);
}

class RequestGetIntJob:public Job
{
    public:
        explicit RequestGetIntJob(GenControl *control)
            : control(control)
        {
            uid = control->m_uid;
        }
        void exec() {
            if (!Job::isRecentlyDeleted(uid)) {
                control->forward(new NewValueEvent(control, control->getInt()));
            }
        }

    protected:
        GenControl * control;
        unsigned int uid;
};

class RequestSetIntJob:public Job
{
    public:
        explicit RequestSetIntJob(GenControl *control, int value)
            : control(control),
            value(value)
        {
            uid = control->m_uid;
        }
        void exec() {
            if (!Job::isRecentlyDeleted(uid)) {
                control->setInt(value);
            }
        }

    protected:
        GenControl * control;
        unsigned int uid;
        int value;
};

template<class T>
void Control<T>::queueGetInt()
{
    Job::push(new RequestGetIntJob(this));
}

template<class T>
void Control<T>::queueSetInt(int value)
{
    Job::push(new RequestSetIntJob(this, value));
}

template<class T>
void Control<T>::addpar(XMLwrapper *xml, const std::string& name)
{
    xml->addpar(name, getInt());
}

template<class T>
void Control<T>::getpar(XMLwrapper *xml, const std::string& name)
{
    setInt(xml->getpar127(name, getInt()));
}

template<class T>
void Control<T>::saveXml(XMLwrapper *xml)
{
    if (options & NoXml) return;
    if (options & NoXmlIfDefault &&
            value == defaultval) return;

    addpar(xml, getId());
    Node::saveXml(xml);
    //xml->addpar(getId(), getInt());
}

template<class T>
void Control<T>::setOptions(ControlOptions opt, bool enable)
{
    if (enable)
        options |= opt;
    else
        options &= ~opt;
}

