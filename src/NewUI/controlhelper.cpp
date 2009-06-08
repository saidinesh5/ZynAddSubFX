#include "controlhelper.h"

ControlHelper::ControlHelper(QObject *parent, Control *control)
	: QObject(parent),
	m_control(control)
{



}

void ControlHelper::controlChanged(Control* control)
{

}
