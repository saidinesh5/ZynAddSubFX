
#ifndef EVENT_H
#define EVENT_H

/**General Event messager class*/
class Event
{
    friend class Node;

public:
    enum ev_type {
        ChangeEvent,
        UpdateEvent,
        CreateNodeEvent,
        RemovalEvent,
        NewNodeEvent,
        NewValueEvent,
        RequestValueEvent,
        MidiEvent,
        OptionsChangedEvent
    };
    virtual ev_type type() const {return internalType;};
    virtual ~Event();

protected:
    Event(enum ev_type ntype);
    enum ev_type internalType;
    bool isOwned;
    static int ref;
};


#endif // EVENT_H
