#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>


typedef std::vector<class Node*>::const_iterator NodeIterator;

class Node
{
    protected:
        virtual std::string doCreateChild(int type);

        std::vector<std::string> m_types;

    public:
        Node(Node* parent, std::string id);
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
        void clearChildren();
        Node* findChild(std::string id);
        const std::vector<std::string> getTypes();

        static inline Node* getRoot() { return m_root; }
        static Node* find(std::string id) { return m_root->findChild(id); }


    private:
        std::string m_id, m_description;
        Node *m_parent;
        std::vector<Node*> m_children;

        static Node* m_root;

};

#endif // NODE_H
