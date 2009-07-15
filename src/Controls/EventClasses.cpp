#include "EventClasses.h"

ChangeEvent::ChangeEvent(char nval)
    :Event(Event::ChangeEvent),val(nval) 
{
};

UpdateEvent::UpdateEvent()
    :Event(Event::UpdateEvent)
{};

RequestValueEvent::RequestValueEvent()
    :Event(Event::RequestValueEvent)
{};


NewValueEvent::NewValueEvent(char val)
    :Event(Event::NewValueEvent),
    val(val)
{};

CreateNodeEvent::CreateNodeEvent(unsigned int ntype)
    :Event(Event::CreateNodeEvent),type(ntype)
{};

NewNodeEvent::NewNodeEvent(class Node *nnode)
    :Event(Event::NewNodeEvent),node(nnode)
{};
