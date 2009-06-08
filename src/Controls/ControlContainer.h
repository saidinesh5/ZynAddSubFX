#ifndef _CONTROLCONTAINER_H_
#define _CONTROLCONTAINER_H_

#include "Control.h"
#include <vector>

class ControlContainer
{

	public:
		ControlContainer(ControlContainer *parent);
		static ControlContainer* getRoot();
		const std::vector<ControlContainer*>& getControls();

	private:
		static ControlContainer* m_root;
		std::vector<ControlContainer*> m_controls;


};

#endif /* ifndef _CONTROLCONTAINER_H_ */
