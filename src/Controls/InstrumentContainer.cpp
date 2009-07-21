#include "InstrumentContainer.h"
#include "../Misc/Master.h"
#include "EventClasses.h"
#include "JobClasses.h"
#include <iostream>

InstrumentContainer::InstrumentContainer(class Node *parent, std::string id, Master* master)
        : Node(parent, id),
        m_master(master),
        nextChildIndex(0),
        nextFakeIndex(0)
{
    m_types.push_back("Part");
}

void InstrumentContainer::handleEvent(Event * ev)
{
    if(ev->type()==Event::CreateNodeEvent) {
    }
    else {
        std::cerr << "Unknown Event Received InstrumentContainer::handleEvent(Event)"
            << std::endl;
    }
}

void InstrumentContainer::handleSyncEvent(Event * ev)
{
    if (ev->type()==Event::CreateNodeEvent) {
        Part* fakeCreatedPart = m_master->part[nextFakeIndex];
        nextFakeIndex++;

        //create a child name
        std::stringstream ss;
        ss << fakeCreatedPart->getId();
        ss << nextChildIndex;
        nextChildIndex++;

        fakeCreatedPart->moveToParent(this);
        fakeCreatedPart->rename(ss.str());

        createdChild = fakeCreatedPart->getAbsoluteId();
    } else if (ev->type() == Event::RemovalEvent) {
        //for this "faked" deletion, nothing to do here
    } else {
        std::cerr << "Unknown Event Received InstrumentContainer::"
            << "handleSyncEvent(Event)" << std::endl;
    }
    cout << "finished handleSyncEvent\n";
}

std::string InstrumentContainer::doCreateChild(int type)
{
    createdChild.clear();

    Job *job = new NodeJob(*this, new CreateNodeEvent(type));
    Job::pushAndWait(job);

    return createdChild;

}
void InstrumentContainer::doRemoveChild(std::string name)
{
    Node *node = Node::find(getAbsoluteId() + "." + name);
    std::cout << "Finding " << name << " returned " << node << "\n";

    if (!node) return;

    Job *job = new NodeJob(*this, new RemovalEvent(node));
    Job::pushAndWait(job);
}
