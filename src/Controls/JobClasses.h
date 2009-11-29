#ifndef JOBCLASSES_H
#define JOBCLASSES_H

#include "Job.h"
#include "Node.h"

class NodeJob:public Job
{
    public:
        explicit NodeJob(Node *nnode, class Event *nev)
            :node(dynamic_cast<Node*>(nnode)), ev(nev) {
                uid = nnode->m_uid;
            }
        class Event *getEvent() const {return ev;}
        void exec() {
            if (!Job::isRecentlyDeleted(uid)) {
                node->handleSyncEvent(ev);
            }
            delete ev;
        }

    protected:
        Node *node;
        class Event * ev;
        unsigned int uid;
};

#endif

