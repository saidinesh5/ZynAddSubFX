#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Job.h"
#include "Event.h"

class NodeUser
{
    public:
        virtual ~NodeUser() {};
        /**Handle given event
         * @param event the pointer to the given event*/
        virtual void handleEvent(Event *event) = 0;
        /**Handle given event durring the Sync time
         * @param event the pointer to the given event*/
        virtual void handleSyncEvent(Event *event) {handleEvent(event);}
};

class RedirectFilter
{
    public:
        virtual ~RedirectFilter() {}
        virtual bool filterEvent(class Event *event) const
        { return false; }
        //perhaps this would work better with a doFilter method
        //It would pass it on to the next filter (which could be an
        //NodeUser)
        //The last filter would pass it on to the NodeUser which
        //was the destination
        //This means that the filters would be creating a singly linked
        //list/chain
};

class TypeFilter:public RedirectFilter
{
    public:
        TypeFilter(Event::ev_type type):type(type) {}

        bool filterEvent(class Event *event) const
        {
            if(event->type() == type)
                return false;
            return true;
        }
    private:
        Event::ev_type type;
};

typedef std::vector<class Node *>::const_iterator NodeIterator;

class Node:public NodeUser
{
    //these are both debug classes
    friend class DebugInterface;
    friend class Tree;
    friend class NodeJob;

    protected:
        virtual std::string doCreateChild(int type);
        virtual void doRemoveChild(std::string name);
        /**Funciton used by subclasses to forward events to registered
         * NodeUsers*/
        void forward(Event *event) const;

        std::vector<std::string> m_types;

    public:
        Node(Node *parent, std::string id);
        virtual ~Node();
        const std::string &getId();
        const std::string getAbsoluteId();
        void printTree();
        const std::vector<Node *> &getChildren() const { return m_children; }
        void moveToParent(Node *parent);
        inline bool hasChildren();

        void rename(std::string newName);
        void addChild(Node *node);
        std::string createChild(int type);
        std::string createChild(std::string name);
        void removeChild(std::string name);
        void clearChildren();
        Node *getChild(std::string id);

        const std::vector<std::string> getTypes();
        bool removeFromParent();

        virtual void handleEvent(Event *ev); //you might want this to stay
                                            //pure virtual
        void addRedirection(NodeUser *destination,
                            RedirectFilter *filter = new RedirectFilter());

        //destination == NULL means delete all redirections
        void removeRedirections(NodeUser *destination);

        static inline Node *getRoot() { return m_root; }
        static void setRoot(Node *root);
        static Node *get(std::string id) { return m_root->getChild(id); }
        static void lock();
        static void unlock();


    private:
        struct Redirection {
            NodeUser *destination;
            RedirectFilter *filter;
        };

        std::vector<Redirection> m_rules;

        //these give us unique id's for every instantiated node. used (so far) for uniquely identifying nodes
        //and their nodejobs together.
        unsigned int m_uid;
        static unsigned int m_nextUid;

        std::string m_id, m_description;
        Node *m_parent;
        std::vector<Node *> m_children;

        static Node *m_root;
};

#endif // NODE_H

