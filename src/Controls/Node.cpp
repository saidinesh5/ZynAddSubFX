#include "Node.h"

#include <iostream>

using std::vector;
using std::string;

Node* Node::m_root = NULL;

void Node::forward(Event event) const
{
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

}

Node::Node(Node* parent, std::string id)
    : m_parent(parent)
{
    rename(id);
    if (m_parent)
        m_parent->addChild(this);
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

const std::vector<std::string> Node::getTypes()
{
    return m_types;
}

string Node::createChild(int type)
{
    return doCreateChild(type);
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

void Node::handleEvent(Event &event)
{
    vector<Redirection>::iterator it;
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

}

void Node::addRedirection(NodeUser *destination, RedirectFilter filter)
{
    Redirection re = {destination, filter};
    m_rules.push_back(re);
}

// vim: sw=4 sts=4 et tw=100
