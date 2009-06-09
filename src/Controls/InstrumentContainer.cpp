#include "InstrumentContainer.h"

InstrumentContainer::InstrumentContainer(class ControlContainer *parent, std::string id)
	: ControlContainer(parent, id)
{
	m_types.push_back("Instrument");
}

std::string InstrumentContainer::createControlContainer(int type)
{

}
