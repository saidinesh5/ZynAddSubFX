#include "InstrumentContainer.h"
#include "../Misc/Master.h"

class InstrumentAdd : public ChildAdded
{
    public:
        Part* m_fakePart;
        std::string m_newName;

        InstrumentAdd(Part* fakeCreatedPart, std::string newName, class ControlContainer *parentContainer, int type)
            : ChildAdded(parentContainer, type)
              , m_fakePart(fakeCreatedPart)
              , m_newName(newName)
        {

        }

        virtual bool exec()
        {
            m_fakePart->container.moveToParent(m_parentContainer);
            m_fakePart->container.rename(m_newName);

            m_childId = m_fakePart->container.getAbsoluteId();
            return false; //false, because we want this event to be bounced back to anyone interested in it

        }
    private:
        class Master* m_master;
};

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

    InstrumentAdd* add = new InstrumentAdd(fakeCreatedPart, ss.str(), this, type);
    Event::pushAndWait(add);

    return add->m_newName;
}
