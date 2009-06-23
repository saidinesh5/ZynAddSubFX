#ifndef _CONTROLCONTAINER_H_
#define _CONTROLCONTAINER_H_

#include "Control.h"
#include "Event.h"
#include <vector>
#include <string>

typedef std::vector<class Control*>::const_iterator ControlIterator;
typedef std::vector<class ControlContainer*>::const_iterator ContainerIterator;

class ChildAdded : public Event
{
public:
    ChildAdded(class ControlContainer *parentContainer, int type);
    std::string getChildId();
    class ControlContainer* getParentContainer();

protected:
    class ControlContainer* m_parentContainer;
    int m_type;
    std::string m_childId;
};

class ControlContainer
{

public:
    ControlContainer(ControlContainer *parent, std::string id);

    const std::string& getId();
    const std::string& getAbsoluteId();
    const std::vector<Control*>& getControls();
    const std::vector<ControlContainer*>& getContainers();
    static ControlContainer* getRoot();

    //convenience method
    static inline Control *find(std::string id) {
        return ControlContainer::getRoot()->findControl(id);
    }
    Control* findControl(std::string id);
    ControlContainer* findContainer(std::string id);

    void addControl(Control* control);
    void addControlContainer(ControlContainer* container);
    void moveToParent(ControlContainer *parent);
    void rename(std::string newName);

    //children-spawning methods
    virtual std::string createControlContainer(int type);
    std::string createControlContainer(std::string name);
    void clear();
    const std::vector<std::string> getTypes();

    void printTree();

protected:
    std::vector<std::string> m_types;
    std::vector<ControlContainer*> m_containers;

private:
    static ControlContainer* m_root;
    class ControlContainer *m_parent;
    std::vector<Control*> m_controls;
    std::string m_id, m_absoluteId;


};

#endif /* ifndef _CONTROLCONTAINER_H_ */
