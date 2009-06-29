#ifndef _INSTRUMENTCONTAINER_H_
#define _INSTRUMENTCONTAINER_H_

#include "Node.h"
#include <string>
#include <vector>

class InstrumentContainer : public Node
{
public:
    InstrumentContainer(class Node* parent, std::string id, class Master *master);

private:
    virtual std::string doCreateChild(int type);
    class Master *m_master;

    int nextChildIndex;

    //TODO* this should be replaced with a dynamic mechanism
    std::vector<int> allocatedInstruments;
    int nextFakeIndex;
};

#endif /* ifndef _INSTRUMENTCONTAINER_H_ */
