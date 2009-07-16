
#ifndef EVENT_H
#define EVENT_H


class ChangeEvent;
class UpdateEvent;
class CreateNodeEvent;
class NewNodeEvent;
class NewValueEvent;
class RequestValueEvent;
class RemovalEvent;

/**General Event messager class*/
class Event
{
    friend class ChangeEvent;
    friend class UpdateEvent;
    friend class NewValueEvent;
    friend class RequestValueEvent;
    friend class CreateNodeEvent;
    friend class NewNodeEvent;
    friend class RemovalEvent;
    friend class Node;

public:
    enum ev_type {ChangeEvent,UpdateEvent,CreateNodeEvent,
        RemovalEvent, NewNodeEvent, NewValueEvent,
        RequestValueEvent};
    virtual ev_type type() const {return internalType;};
    virtual ~Event();

private:
    Event(enum ev_type ntype);
    enum ev_type internalType;
    bool isOwned;
    static int ref;
};


#endif // EVENT_H
