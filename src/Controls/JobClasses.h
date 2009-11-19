#ifndef JOBCLASSES_H
#define JOBCLASSES_H

#include "Job.h"
#include "Node.h"

class NodeJob:public Job
{
    public:
        explicit NodeJob(NodeUser *nnode, class Event *nev)
            :node(dynamic_cast<NodeUser*>(nnode)), ev(nev) {
            }
        class Event *getEvent() const {return ev;}
        void exec() {
            node->handleSyncEvent(ev);
            delete ev;
        }

    protected:
        NodeUser *node;
        class Event * ev;
};

#endif

