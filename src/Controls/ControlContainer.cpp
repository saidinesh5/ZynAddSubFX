#include "ControlContainer.h"

#include <string>

using std::vector;
using std::string;

ControlContainer* ControlContainer::m_root = new ControlContainer(NULL, "Master");

ControlContainer::ControlContainer(ControlContainer* parent, string id)
	: m_id(id)
{
	if (parent)
		m_absoluteId = parent->getAbsoluteId() + "." + id;
	else
		m_absoluteId = id;
}

ControlContainer* ControlContainer::getRoot()
{
	return m_root;
}


const vector<Control*>& ControlContainer::getControls()
{
	return m_controls;
}

void ControlContainer::addControl(Control* control)
{
	m_controls.push_back(control);
}

void ControlContainer::addControlContainer(ControlContainer* container)
{
	m_containers.push_back(container);
}

Control* ControlContainer::findControl(std::string id)
{
	for (ControlIterator it = m_controls.begin();
			it != m_controls.end();
			it++)
	{
		if (id == (*it)->getAbsoluteId())
			return *it;
	}


	Control *ret = NULL;
	for (ContainerIterator it = m_containers.begin();
			it != m_containers.end();
			it++)
	{
		//see if the absolute id of the container matches with the
		//beginning of the id being searched for.
		int pos = id.find((*it)->getAbsoluteId());
		//pos == 0 is a match. if no match, continue.
		if (pos) continue; 

		ret = (*it)->findControl(id);
		if (ret) return ret;
	}
	return ret;
}

const std::string& ControlContainer::getId()
{
	return m_id;
}

const std::string& ControlContainer::getAbsoluteId()
{
	return m_absoluteId;
}
