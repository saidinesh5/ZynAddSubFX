#ifndef EVENTCLASSES_H
#define EVENTCLASSES_H

class ChildAdded : public Event
{
public:
    ChildAdded(class Node *parentContainer, int type) : m_parentContainer(parentContainer),
    m_type(type) {}
    std::string getChildId() { return m_childId; }
    class Node* getParentContainer() { return m_parentContainer; }

protected:
    class Node* m_parentContainer;
    int m_type;
    std::string m_childId;
};

#endif // EVENTCLASSES_H
