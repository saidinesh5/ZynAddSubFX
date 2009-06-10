#ifndef _INSTRUMENTCONTAINER_H_
#define _INSTRUMENTCONTAINER_H_

#include "ControlContainer.h"
#include <string>

class InstrumentContainer : public ControlContainer
{
	public:
		InstrumentContainer(class ControlContainer* parent, std::string id, class Master *master);
		virtual std::string createControlContainer(int type);

	private:
		class Master *m_master;
};

#endif /* ifndef _INSTRUMENTCONTAINER_H_ */
