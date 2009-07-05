#include "EventClasses.h"
    
ChangeEvent::ChangeEvent(unsigned char nval)
    :Event(Event::ChangeEvent),val(nval) 
{};

UpdateEvent::UpdateEvent()
    :Event(Event::UpdateEvent)
{};

CreateNodeEvent::CreateNodeEvent(unsigned int ntype)
    :Event(Event::CreateNodeEvent),type(ntype)
{};

NewNodeEvent::NewNodeEvent(class Node *nnode)
    :Event(Event::NewNodeEvent),node(nnode)
{};
