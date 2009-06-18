#include "ControlContainer.h"

#include <string>
#include <iostream>

using std::vector;
using std::string;

ControlContainer* ControlContainer::m_root = new ControlContainer(NULL, "Master");

ControlContainer::ControlContainer(ControlContainer* parent, string id)
	: m_id(id),
	m_parent(parent)
{
	rename(id);
	if (m_parent)
		m_parent->addControlContainer(this);
}

ControlContainer* ControlContainer::getRoot()
{
	return m_root;
}


const vector<Control*>& ControlContainer::getControls()
{
	return m_controls;
}

const vector<ControlContainer*>& ControlContainer::getContainers()
{
	return m_containers;
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

void ControlContainer::printTree()
{
	std::cout << "Container " << getAbsoluteId() << std::endl;

	for (ControlIterator it = m_controls.begin();
			it != m_controls.end();
			it++)
	{
		std::cout << "Control " << (*it)->getAbsoluteId() << std::endl;
	}


	Control *ret = NULL;
	for (ContainerIterator it = m_containers.begin();
			it != m_containers.end();
			it++)
	{
		(*it)->printTree();
	}
}

ControlContainer* ControlContainer::findContainer(std::string id)
{
	ControlContainer *ret = NULL;
	for (ContainerIterator it = m_containers.begin();
			it != m_containers.end();
			it++)
	{

		if (id == (*it)->getAbsoluteId()) return *it;

		//see if the absolute id of the container matches with the
		//beginning of the id being searched for.
		int pos = id.find((*it)->getAbsoluteId());
		//pos == 0 is a match. if no match, continue.
		if (pos) continue; 

		ret = (*it)->findContainer(id);
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

const std::vector<std::string> ControlContainer::getTypes()
{
	return m_types;
}

std::string ControlContainer::createControlContainer(int type)
{
	return "";
}

std::string ControlContainer::createControlContainer(std::string name)
{
	//convenience function
	for (int i = 0; i < m_types.size(); ++i) {
		if (m_types.at(i) == name)
			return createControlContainer(i);
	}
	return "";
}

void ControlContainer::clear()
{
	m_containers.clear();
}

void ControlContainer::moveToParent(ControlContainer *parent)
{
	if (m_parent) {
		for (vector<ControlContainer*>::iterator it = m_parent->m_containers.begin(); it != m_parent->m_containers.end(); it++) {
			if (*it == this) {
				m_parent->m_containers.erase(it);
				break;
			}
		}
	}

	m_parent = parent;
	rename(m_id);

	m_parent->addControlContainer(this);
}

void ControlContainer::rename(std::string newName)
{
	m_id = newName;
	if (m_parent)
		m_absoluteId = m_parent->getAbsoluteId() + "." + m_id;
	else
		m_absoluteId = m_id;
}

ChildAdded::ChildAdded(class ControlContainer *parentContainer, int type)
    : m_parentContainer(parentContainer),
    m_type(type)
{

}

std::string ChildAdded::getChildId()
{
    return m_childId;
}

ControlContainer* ChildAdded::getParentContainer()
{
    return m_parentContainer;
}
