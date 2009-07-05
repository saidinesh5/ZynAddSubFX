#include "CharControl.h"
#include <iostream>
#include "Job.h"

using std::string;

CharControl::CharControl(Node *parent, string id, string description, char min, char max, char defaultValue) :
        Control(parent,id, description, 0),
        m_max(max),
        m_min(min),
        m_default(defaultValue),
        m_value(defaultValue)
{

}

string CharControl::getString() const
{
    return "lala";
}

char CharControl::getValue() const
{
    return m_value;
}

class SetCharJob : public Job
{
public:
    char* m_source, m_value;
    CharValueFunctor functor;

    void exec() {
        *m_source = m_value;
        functor.valueSet(m_value);
        std::cout << "set " << m_source << " to " << m_value << std::endl;
    }
};

void CharControl::setValue(char nval)
{
    SetCharJob *event = new SetCharJob;
    event->m_source = &m_value;
    event->m_value = nval;
    event->functor = functor;
    Job::push(event);
}

void CharControl::setFunctor(CharValueFunctor functor)
{
    this->functor = functor;
}
