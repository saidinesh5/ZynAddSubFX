#include "ControlContainer.h"

using std::vector;

ControlContainer* ControlContainer::m_root = new ControlContainer(NULL);

ControlContainer::ControlContainer(ControlContainer* parent)
{


}

ControlContainer* ControlContainer::getRoot()
{
	return m_root;
}


const vector<ControlContainer*>& ControlContainer::getControls()
{
	return m_controls;
}
