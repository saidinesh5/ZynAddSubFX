#ifndef _CONTROLCONTAINER_H_
#define _CONTROLCONTAINER_H_

#include "Control.h"
#include <vector>
#include <string>

typedef std::vector<class Control*>::const_iterator ControlIterator;
typedef std::vector<class ControlContainer*>::const_iterator ContainerIterator;

class ControlContainer
{

	public:
		ControlContainer(ControlContainer *parent, std::string id);
		const std::vector<Control*>& getControls();

		static ControlContainer* getRoot();
		//convenience method
		static inline Control *find(std::string id) { 
			return ControlContainer::getRoot()->findControl(id); }
		
		const std::string& getId();
		const std::string& getAbsoluteId();
		
		Control* findControl(std::string id);
		void addControl(Control* control);
		void addControlContainer(ControlContainer* container);

		virtual std::string createControlContainer(int type);
		std::string createControlContainer(std::string name);
		const std::vector<std::string> getTypes();
			
	protected:
		std::vector<std::string> m_types;

	private:
		static ControlContainer* m_root;
		std::vector<Control*> m_controls;
		std::vector<ControlContainer*> m_containers;
		std::string m_id, m_absoluteId;


};

#endif /* ifndef _CONTROLCONTAINER_H_ */
