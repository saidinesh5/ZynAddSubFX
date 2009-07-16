#include "Node.h"

#include <iostream>
#include "EventClasses.h"

using std::vector;
using std::string;

Node* Node::m_root = NULL;


Node::Node(Node* parent, std::string id)
    : m_parent(parent)
{
    rename(id);
    if (m_parent)
        m_parent->addChild(this);
}

Node::~Node()
{



    //if (m_parent) {
        //forward(new RemovalEvent(this));
    //}

    for (vector<Node*>::iterator it = m_children.begin(); it != m_children.end(); it++) {
        delete (*it);
    }
}

void Node::forward(Event *event) const
{
    bool deleteIt = false;

    if (!event->isOwned) {
        event->isOwned = true;
        deleteIt = true;
    }

    vector<Redirection>::const_iterator it;
    for (it = m_rules.begin(); it != m_rules.end(); it++) {

        //TODO: handle multiple filters here if needed
        //RETODO: I think it might be better to have one smart filter
        //        rather than a filter chain
        if ((*it).filter.filterEvent(event)) {
            continue; //event was filtered out
        }

        //event passed the filter, so
        //pass it on to the next node
        (*it).destination->handleEvent(event);

    }

    if (deleteIt) {
        delete event;
    }

}


const std::string& Node::getId()
{
    return m_id;
}

const std::string Node::getAbsoluteId()
{
    if (!m_parent) return getId();
    return m_parent->getAbsoluteId() + "." + m_id;

}

void Node::rename(std::string newName)
{
    m_id = newName;
}

void Node::addChild(Node* node)
{
    m_children.push_back(node);
    node->addRedirection(this, TypeFilter(Event::RemovalEvent));
}

bool Node::hasChildren()
{
    return !m_children.empty();
}

void Node::moveToParent(Node *parent)
{
    if (m_parent) {
        for (vector<Node*>::iterator it = m_parent->m_children.begin(); it != m_parent->m_children.end(); it++) {
            if (*it == this) {
                m_parent->m_children.erase(it);
                break;
            }
        }
    }

    m_parent = parent;
    rename(m_id);

    m_parent->addChild(this);
}

Node* Node::findChild(std::string id)
{
    Node *ret = NULL;
    for (NodeIterator it = m_children.begin();
            it != m_children.end();
            it++) {

        if (id == (*it)->getAbsoluteId()) return *it;

        //see if the absolute id of the container matches with the
        //beginning of the id being searched for.
        int pos = id.find((*it)->getAbsoluteId());
        //pos == 0 is a match. if no match, continue.
        if (pos) continue;

        if (!(*it)->hasChildren()) continue;

        ret = (*it)->findChild(id);
        if (ret) return ret;
    }
    return ret;
}

void Node::clearChildren()
{
    m_children.clear();
}

string Node::doCreateChild(int type)
{
    return string();
}

void Node::doRemoveChild(std::string name)
{

}

const std::vector<std::string> Node::getTypes()
{
    return m_types;
}

string Node::createChild(int type)
{
    return doCreateChild(type);
}

void Node::removeChild(std::string name)
{
    doRemoveChild(name);
}

string Node::createChild(string name)
{
    //convenience function
    for (int i = 0; i < m_types.size(); ++i) {
        if (m_types.at(i) == name)
            return createChild(i);
    }
    return string();
}

void Node::printTree()
{
    std::cout << getAbsoluteId() << std::endl;

    Node *ret = NULL;
    for (NodeIterator it = m_children.begin();
            it != m_children.end();
            it++) {

        (*it)->printTree();
    }
}

void Node::handleEvent(Event *event)
{
    forward(event);
}

void Node::addRedirection(NodeUser *destination, RedirectFilter filter)
{
    Redirection re = {destination, filter};
    m_rules.push_back(re);
}

void Node::removeRedirections(NodeUser *destination)
{
    vector<Redirection>::iterator it;
    for (it = m_rules.begin(); it != m_rules.end(); it++) {
        if ((*it).destination == destination)
            m_rules.erase(it);
    }
}


// vim: sw=4 sts=4 et tw=100
