#include "CharControl.h"

using std::string;

CharControl::CharControl(ControlContainer *parent,string desc, char min, char max, char defaultValue) : 
	Control(parent, 0),
	m_desc(desc),
	m_max(max),
	m_min(min),
	m_default(defaultValue),
	m_value(defaultValue)
{

}

string CharControl::getString() const
{
	return m_desc;
}

char CharControl::getValue() const
{
	return m_value;
}

void CharControl::setValue(char nval)
{
	m_value = nval;
}
