
#ifndef EVENT_H
#define EVENT_H


class ChangeEvent;
class UpdateEvent;
class CreateNodeEvent;
class NewNodeEvent;

/**General Event messager class*/
class Event
{
    friend class ChangeEvent;
    friend class UpdateEvent;
    friend class CreateNodeEvent;
    friend class NewNodeEvent;

public:
    enum ev_type {ChangeEvent,UpdateEvent,CreateNodeEvent,
    NewNodeEvent};
    virtual ev_type type() const {return internalType;};

private:
    Event(enum ev_type ntype):internalType(ntype) {};
    enum ev_type internalType;
};


#endif // EVENT_H
