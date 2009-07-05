#ifndef _INSTRUMENTCONTAINER_H_
#define _INSTRUMENTCONTAINER_H_

#include "Node.h"
#include "Event.h"
#include "EventClasses.h"
#include <string>
#include <vector>

class InstrumentContainer : public Node
{
public:
    InstrumentContainer(class Node* parent, std::string id, class Master *master);
    void handleEvent(Event& ev);
    void handleSyncEvent(Event& ev);

private:
    virtual std::string doCreateChild(int type);
    class Master *m_master;

    int nextChildIndex;

    //TODO* this should be replaced with a dynamic mechanism
    std::vector<int> allocatedInstruments;
    int nextFakeIndex;
    std::string createdChild;
};

#endif /* ifndef _INSTRUMENTCONTAINER_H_ */
