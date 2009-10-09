#ifndef FAKECHILDFACTORY_H
#define FAKECHILDFACTORY_H

#include "Node.h"
#include <string>
#include <list>

class FakeChildFactory:public Node
{
    public:
        FakeChildFactory(Node *parent, std::string id);

        inline void operator<<(Node *node) { addFakeChild(node); }
        void addFakeChild(Node *node);
        void addType(std::string type);
        void handleEvent(class Event * ev);
        void handleSyncEvent(class Event * ev);

    private:
        virtual std::string doCreateChild(int type);
        virtual void doRemoveChild(std::string name);
        std::list<Node *> fakeChildren;
        int nextChildIndex;
        std::string createdChild;
};

#endif // FAKECHILDFACTORY_H

