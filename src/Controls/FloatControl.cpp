#include "FloatControl.h"

using std::string;

FloatControl::FloatControl(ControlContainer *parent,string id, string description, float min, float max, float defaultValue) : 
	Control(parent, id, description, 0),
	m_max(max),
	m_min(min),
	m_default(defaultValue),
	m_value(defaultValue)
{

}

string FloatControl::getString() const
{
	return "lala";
}

float FloatControl::toFloat(char val) const
{
	return m_min + (m_max - m_min) * (float(val) / 128);
}

char FloatControl::toChar(float val) const
{
	return char((val - m_min) / (m_max - m_min) * 128);
}

char FloatControl::getValue() const
{
	return toChar(m_value);
}

void FloatControl::setValue(char nval)
{
	m_value = toFloat(nval);
}
