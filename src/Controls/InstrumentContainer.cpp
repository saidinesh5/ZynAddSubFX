#include "InstrumentContainer.h"
#include "../Misc/Master.h"

InstrumentContainer::InstrumentContainer(class ControlContainer *parent, std::string id, Master* master)
	: ControlContainer(parent, id),
	m_master(master)
{
	m_types.push_back("Instrument");
}

std::string InstrumentContainer::createControlContainer(int type)
{

}
