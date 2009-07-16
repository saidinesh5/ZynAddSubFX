#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Job.h"
#include "Event.h"

class NodeUser
{
public:
    /**Handle given event
     * @param event the pointer to the given event*/
    virtual void handleEvent(Event *event) = 0;
    /**Handle given event durring the Sync time
     * @param event the pointer to the given event*/
    virtual void handleSyncEvent(Event *event){handleEvent(event);};
};

class RedirectFilter
{
public:
    virtual bool filterEvent(class Event * event) const
        { return false; }
        //perhaps this would work better with a doFilter method
        //It would pass it on to the next filter (which could be an
        //NodeUser)
        //The last filter would pass it on to the NodeUser which
        //was the destination
        //This means that the filters would be creating a singly linked
        //list/chain
};

class TypeFilter : public RedirectFilter
{
    public:
        TypeFilter(Event::ev_type type) : type(type) {}

        bool filterEvent(class Event * event) const
        {
            if (event->type() == type) return true;
            return false;
        }
    private:
        Event::ev_type type;
};

typedef std::vector<class Node*>::const_iterator NodeIterator;

class Node: public NodeUser
{
    //these are both debug classes
    friend class DebugInterface;
    friend class Tree;

    protected:
        virtual std::string doCreateChild(int type);
        virtual void doRemoveChild(std::string name);
        /**Funciton used by subclasses to forward events to registered
         * NodeUsers*/
        void forward(Event *event)const;

        std::vector<std::string> m_types;

    public:
        Node(Node* parent, std::string id);
        virtual ~Node();
        const std::string& getId();
        const std::string getAbsoluteId();
        void printTree();
        const std::vector<Node*>& getChildren() const { return m_children; }
        void moveToParent(Node *parent);
        inline bool hasChildren();

        void rename(std::string newName);
        void addChild(Node* node);
        std::string createChild(int type);
        std::string createChild(std::string name);
        void removeChild(std::string name);
        void clearChildren();
        Node* findChild(std::string id);//should be getChild(string) find seems
                                        //to be misleading 
        const std::vector<std::string> getTypes();

        virtual void handleEvent(Event *ev);//you might want this to stay
                                             //pure virtual
        void addRedirection(NodeUser *destination, RedirectFilter filter = RedirectFilter());
        void removeRedirections(NodeUser *destination);

        static inline Node* getRoot() { return m_root; }
        static void setRoot(Node* root) { m_root = root; }
        static Node* find(std::string id) { return m_root->findChild(id); }


    private:
        struct Redirection
        {
            NodeUser *destination;
            RedirectFilter filter;
        };

        std::vector<Redirection> m_rules;

        std::string m_id, m_description;
        Node *m_parent;
        std::vector<Node*> m_children;

        static Node* m_root;

};

#endif // NODE_H
