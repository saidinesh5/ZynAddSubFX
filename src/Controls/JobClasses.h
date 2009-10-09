#ifndef JOBCLASSES_H
#define JOBCLASSES_H

#include "Job.h"
#include "Node.h"

class NodeJob:public Job
{
    public:
        NodeJob(NodeUser &nnode, class Event *nev)
            :node(nnode), ev(nev) {}
        class Event *getEvent() const {return ev;}
        NodeUser &getNodeUser() const {return node;}
        void exec() {
            node.handleSyncEvent(ev);
            delete ev;
        }

    protected:
        NodeUser &node;
        class Event * ev;
};

#endif

