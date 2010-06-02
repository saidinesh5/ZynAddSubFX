#include "Node.h"

#include <iostream>
#include "EventClasses.h"
#include <assert.h>

#include <pthread.h>
#include "../globals.h"
#include "../Misc/XMLwrapper.h"

using namespace std;

Node *Node::m_root = NULL;
static pthread_mutex_t treeMutex;

unsigned int Node::m_nextUid = 0;

Node::Node(Node *parent, std::string id)
    : m_uid(m_nextUid),
    m_parent(parent)
{
    m_nextUid++;

    rename(id);
    if(m_parent)
        m_parent->addChild(this);
}

Node::~Node()
{
    Job::addToRecentlyDeleted(m_uid);

    for(vector<Node *>::iterator it = m_children.begin();
        it != m_children.end();
        it++)
        m_children.erase(it);

    if(m_parent) {
        m_parent->removeRedirections(this);
        for(vector<Node *>::iterator it = m_parent->m_children.begin();
            it != m_parent->m_children.end();
            it++)
            if(*it == this) {
                m_parent->m_children.erase(it);
                break;
            }
    }
    forward(new RemovalEvent(this));

    removeRedirections(NULL);
    m_rules.clear();
}

void Node::forward(Event *event) const
{
    bool deleteIt = false;

    if(!event->isOwned) {
        event->isOwned = true;
        deleteIt = true;
    }

    vector<Redirection>::const_iterator it;
    for(it = m_rules.begin(); it != m_rules.end(); it++) {
        //TODO: handle multiple filters here if needed
        //RETODO: I think it might be better to have one smart filter
        //        rather than a filter chain
        if((*it).filter->filterEvent(event))
            continue; //event was filtered out

        //event passed the filter, so
        //pass it on to the next node
        (*it).destination->handleEvent(event);
    }

    if(deleteIt)
        delete event;

}


const std::string &Node::getId()
{
    return m_id;
}

const std::string Node::getAbsoluteId()
{
    if(!m_parent)
        return getId();
    return m_parent->getAbsoluteId() + "." + m_id;
}

void Node::rename(std::string newName)
{
    m_id = newName;
}

void Node::addChild(Node *node)
{
    m_children.push_back(node);
    //node->addRedirection(this, new TypeFilter(Event::RemovalEvent));
}

bool Node::hasChildren()
{
    return !m_children.empty();
}

void Node::moveToParent(Node *parent)
{
    if(m_parent)
        for(vector<Node *>::iterator it = m_parent->m_children.begin();
            it != m_parent->m_children.end();
            it++)
            if(*it == this) {
                m_parent->m_children.erase(it);
                break;
            }

    m_parent = parent;
    rename(m_id);

    if(m_parent)
        m_parent->addChild(this);
}

Node *Node::getChild(string id)
{
    Node *ret = NULL;
    for(NodeIterator it = m_children.begin();
        it != m_children.end();
        it++) {
        if(id == (*it)->getAbsoluteId())
            return *it;

        //see if the absolute id of the container matches with the
        //beginning of the id being searched for.
        int pos = id.find((*it)->getAbsoluteId());
        //pos == 0 is a match. if no match, continue.
        if(pos)
            continue;

        if(!(*it)->hasChildren())
            continue;

        ret = (*it)->getChild(id);
        if(ret)
            return ret;
    }
    return ret;
}

void Node::clearChildren()
{
    m_children.clear();
}

string Node::doCreateChild(int /*type*/)
{
    return string();
}

void Node::doRemoveChild(std::string /*name*/)
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
    for(unsigned int i = 0; i < m_types.size(); ++i)
        if(m_types.at(i) == name)
            return createChild(i);
    return string();
}

void Node::printTree()
{
    std::cout << getAbsoluteId() << std::endl;

    for(NodeIterator it = m_children.begin();
        it != m_children.end();
        it++)

        (*it)->printTree();
}

void Node::handleEvent(Event *event)
{
    forward(event);
}

void Node::addRedirection(NodeUser *destination, RedirectFilter *filter)
{
    Redirection re = {destination, filter};
    m_rules.push_back(re);
}

void Node::removeRedirections(NodeUser *destination)
{
    vector<Redirection>::iterator it;
    it = m_rules.begin();
    while(it != m_rules.end()) {
        if(!destination
           || ((*it).destination == destination)) {
            delete (*it).filter;
            it = m_rules.erase(it);
        }
        else
            it++;
    }
}

bool Node::removeFromParent()
{
    if(!m_parent)
        return false;

    m_parent->removeChild(getId());
    return true;
}

void Node::setRoot(Node *root)
{
    m_root = root;

    static int once = 0;
    if (!once) {
        once = 1;
        pthread_mutex_init(&treeMutex, NULL);
    }
}

void Node::lock()
{
    pthread_mutex_lock(&treeMutex);
}

void Node::unlock()
{
    pthread_mutex_unlock(&treeMutex);
}

unsigned int Node::getUid() const
{
    return m_uid;
}

void Node::saveXml(XMLwrapper *xml)
{
    if (hasChildren()) {
        xml->beginbranch(getId());
        for(NodeIterator it = m_children.begin();
                it != m_children.end();
                it++) {
            (*it)->saveXml(xml);
        }
        xml->endbranch();
    }
}

// vim: sw=4 sts=4 et tw=100

