#include "InstrumentContainer.h"
#include "../Misc/Master.h"

InstrumentContainer::InstrumentContainer(class ControlContainer *parent, std::string id, Master* master)
	: ControlContainer(parent, id),
	m_master(master),
	nextChildIndex(0),
	nextFakeIndex(0)
{
	m_types.push_back("Part");
}

std::string InstrumentContainer::createControlContainer(int type)
{
	Part* fakeCreatedPart = m_master->part[nextFakeIndex];
	nextFakeIndex++;

	//create a child name
	std::stringstream ss;
	ss << fakeCreatedPart->container.getId();
	ss << nextChildIndex;
	nextChildIndex++;

	fakeCreatedPart->container.moveToParent(this);
	fakeCreatedPart->container.rename(ss.str());

	return fakeCreatedPart->container.getAbsoluteId();
}
