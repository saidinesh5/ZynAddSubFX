#include "FakeChildFactory.h"
#include "Event.h"
#include "EventClasses.h"
#include "JobClasses.h"
#include <iostream>
#include <sstream>
#include <string>

FakeChildFactory::FakeChildFactory(Node *parent, std::string id)
    : Node(parent, id),
    nextChildIndex(1)
{

}

void FakeChildFactory::addFakeChild(Node *node)
{
    fakeChildren.push_back(node);
}

void FakeChildFactory::addType(std::string type)
{
    m_types.push_back(type);
}

std::string FakeChildFactory::doCreateChild(int type)
{
    createdChild.clear();

    Job *job = new NodeJob(*this, new CreateNodeEvent(type));
    Job::pushAndWait(job);

    return createdChild;
}

void FakeChildFactory::doRemoveChild(std::string name)
{
    //TODO: using Node::find here might be thread-unsafe. should be checked
    Node *node = Node::find(getAbsoluteId() + "." + name);
    std::cout << "Finding " << name << " returned " << node << "\n";

    if (!node) return;

    Job *job = new NodeJob(*this, new RemovalEvent(node));
    Job::pushAndWait(job);
}

void FakeChildFactory::handleEvent(Event * ev)
{
    if(ev->type()==Event::CreateNodeEvent) {
    }
    else {
        std::cerr << "Unknown Event Received InstrumentContainer::handleEvent(Event)"
            << std::endl;
    }
}

void FakeChildFactory::handleSyncEvent(Event * ev)
{
    if (ev->type()==Event::CreateNodeEvent) {
        Node *freshNewNode = fakeChildren.front();
        fakeChildren.pop_front();

        freshNewNode->moveToParent(NULL);

        //create a child name
        std::stringstream ss;
        ss << freshNewNode->getId();
        ss << nextChildIndex;
        nextChildIndex++;

        freshNewNode->moveToParent(this);
        freshNewNode->rename(ss.str());

        createdChild = freshNewNode->getAbsoluteId();
    } else if (ev->type() == Event::RemovalEvent) {
        RemovalEvent *removal = dynamic_cast<RemovalEvent*>(ev);

        //remove it from the node tree
        removal->getNode()->moveToParent(NULL);
        fakeChildren.push_back(removal->getNode());
    } else {
        std::cerr << "Unknown Event Received InstrumentContainer::"
            << "handleSyncEvent(Event)" << std::endl;
    }
}

// vim: sw=4 sts=4 et tw=100
