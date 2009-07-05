#include "InstrumentContainer.h"
#include "../Misc/Master.h"
#include "EventClasses.h"
#include "JobClasses.h"
#include <iostream>

//class InstrumentAdd : public AddChild
//{
//public:
    //Part* m_fakePart;
    //std::string m_newName;

    //InstrumentAdd(Part* fakeCreatedPart, std::string newName, class Node *parentContainer, int type)
            //: AddChild(parentContainer, type)
            //, m_fakePart(fakeCreatedPart)
            //, m_newName(newName) {

    //}

    //virtual void exec() {
        //m_fakePart->container.moveToParent(m_parentContainer);
        //m_fakePart->container.rename(m_newName);

        //m_childId = m_fakePart->container.getAbsoluteId();

    //}
//private:
    //class Master* m_master;
//};

InstrumentContainer::InstrumentContainer(class Node *parent, std::string id, Master* master)
        : Node(parent, id),
        m_master(master),
        nextChildIndex(0),
        nextFakeIndex(0)
{
    m_types.push_back("Part");
}

void InstrumentContainer::handleEvent(Event& ev)
{
    if(ev.type()==Event::CreateNodeEvent) {
    }
    else {
        std::cerr << "Unknown Event Received InstrumentContainer::handleEvent(Event)"
            << std::endl;
    }
}

void InstrumentContainer::handleSyncEvent(Event& ev)
{
    cout << "sync event " << &ev << "\n";
    if (ev.type()==Event::CreateNodeEvent) {
        Part* fakeCreatedPart = m_master->part[nextFakeIndex];
        nextFakeIndex++;

        //create a child name
        std::stringstream ss;
        ss << fakeCreatedPart->container.getId();
        ss << nextChildIndex;
        nextChildIndex++;

        fakeCreatedPart->container.moveToParent(this);
        fakeCreatedPart->container.rename(ss.str());

        createdChild = fakeCreatedPart->container.getAbsoluteId();
    }
    else {
        std::cerr << "Unknown Event Received InstrumentContainer::"
            << "handleSyncEvent(Event)" << std::endl;
    }
    cout << "finished handleSyncEvent\n";
}

std::string InstrumentContainer::doCreateChild(int type)
{
    createdChild.clear();

    Job *job = new NodeJob(*this, CreateNodeEvent(type));
    Job::pushAndWait(job);

    return createdChild;

}
