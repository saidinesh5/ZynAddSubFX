#include "FloatControl.h"
#include "../globals.h"
#include "math.h"

using std::string;

FloatControl::FloatControl(ControlContainer *parent,string id, string description, float min, float max, float defaultValue) : 
	Control(parent, id, description, 0),
	m_max(max),
	m_min(min),
	m_default(defaultValue),
	m_value(defaultValue),
	m_db2rapConversion(false)
{

}

string FloatControl::getString() const
{
	return "lala";
}

float FloatControl::toFloat(char val) const
{
	if (m_db2rapConversion) return dB2rap(m_min + (m_max - m_min) * (float(val) / 128));
	return m_min + (m_max - m_min) * (float(val) / 128);
}

char FloatControl::toChar(float val) const
{
	if (m_db2rapConversion) return rap2dB(char((val - m_min) / (m_max - m_min) * 128));
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

float FloatControl::getFloat() const
{
	return m_value;
}

void FloatControl::setDb2rapConversion(bool value)
{
	m_db2rapConversion = value;
}