#include "Control.h"

Control::Control(ControlContainer *parent, std::string id, std::string description, char ndefaultval)
  :defaultval(ndefaultval),
	lockqueue(-1),
	locked(false),
	m_id(id),
	m_description(description),
	m_parent(parent)
{
	parent->addControl(this);
}

void Control::lock()
{
    locked=true;
    lockqueue=-1;
}

void Control::ulock()
{
    if(locked&&lockqueue>=0)
        setValue(lockqueue);
    locked=false;
}

void Control::registerUser(class ControlUser *user)
{
	m_users.push_back(user);
}

const std::string& Control::getId()
{
	return m_id;
}

const std::string Control::getAbsoluteId()
{
	return m_parent->getAbsoluteId() + "." + m_id;

}

