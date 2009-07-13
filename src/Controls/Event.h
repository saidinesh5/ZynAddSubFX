
#ifndef EVENT_H
#define EVENT_H


class ChangeEvent;
class UpdateEvent;
class CreateNodeEvent;
class NewNodeEvent;
class NewValueEvent;
class RequestValueEvent;

/**General Event messager class*/
class Event
{
    friend class ChangeEvent;
    friend class UpdateEvent;
    friend class NewValueEvent;
    friend class RequestValueEvent;
    friend class CreateNodeEvent;
    friend class NewNodeEvent;

public:
    enum ev_type {ChangeEvent,UpdateEvent,CreateNodeEvent,
    NewNodeEvent, NewValueEvent, RequestValueEvent};
    virtual ev_type type() const {return internalType;};

private:
    Event(enum ev_type ntype):internalType(ntype) {};
    enum ev_type internalType;
};


#endif // EVENT_H
