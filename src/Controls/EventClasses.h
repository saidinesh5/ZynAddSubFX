#ifndef EVENTCLASSES_H
#define EVENTCLASSES_H

#include "Event.h"
#include "GenControl.h"

//class AddChild : public Job
//{
//public:
//    AddChild(class Node *parentContainer, int type) : m_parentContainer(parentContainer),
//    m_type(type) {}
//    std::string getChildId() { return m_childId; }
//    class Node* getParentContainer() { return m_parentContainer; }
//
//protected:
//    class Node* m_parentContainer;
//    int m_type;
//    std::string m_childId;
//};

/**Events for setting values at nodes*/
class ChangeEvent: public Event
{

public:
    ChangeEvent(char nval);
    char getVal() const {return val;};
private:
    const char val;
};

/**Notification Event*/
class UpdateEvent: public Event
{
public:
    UpdateEvent();
};

/** Emitted from the node when its value has changed **/
class NewValueEvent : public Event
{
    public:
        NewValueEvent(char val = -1);
        const char val;
};

/** Used for requesting a node to emit its value (ie. a NewValueEvent */
class RequestValueEvent : public Event
{
    public:
        RequestValueEvent();
};

/**Event for Child creation*/
class CreateNodeEvent: public Event
{
public:
    //I recommend that an enum gets put into ntype
    //ie. an Instrument could have an enum of
    //ADsynth=0,SUBsynth=1,PADsynth=2
    //This means that an int with more meaning is passed here
    CreateNodeEvent(unsigned int ntype);
    unsigned int getType() const {return type;};
private:
    const unsigned int type;
};

/* Notification that a node is about to be removed */
class RemovalEvent : public Event
{
    public:
        RemovalEvent(Node *node);
        Node * getNode() const { return node; }
    private:
        Node* node;
};

/**Event for notification of the creation of a new node*/
class NewNodeEvent: public Event
{
public:
    NewNodeEvent(class Node *nnode);
    class Node *getNode(){return node;}
private:
    class Node *node;
};
#endif // EVENTCLASSES_H
