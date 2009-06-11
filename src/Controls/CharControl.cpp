#include "CharControl.h"
#include <iostream>

using std::string;

CharControl::CharControl(ControlContainer *parent, string id, string description, char min, char max, char defaultValue) : 
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

void CharControl::setValue(char nval)
{
	std::cout << "Value set to " << (int)nval << "\n";
	m_value = nval;
}
