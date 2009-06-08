#include "Control.h"

Control::Control(ControlContainer *parent, char ndefaultval)
  :defaultval(ndefaultval),lockqueue(-1),locked(false)
{

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
