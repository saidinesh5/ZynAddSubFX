#ifndef _INSTRUMENTCONTAINER_H_
#define _INSTRUMENTCONTAINER_H_

#include "ControlContainer.h"
#include <string>
#include <vector>

class InstrumentContainer : public ControlContainer
{
	public:
		InstrumentContainer(class ControlContainer* parent, std::string id, class Master *master);
		virtual std::string createControlContainer(int type);

	private:
		class Master *m_master;

		int nextChildIndex;

		//TODO* this should be replaced with a dynamic mechanism
		std::vector<int> allocatedInstruments;
		int nextFakeIndex;
};

#endif /* ifndef _INSTRUMENTCONTAINER_H_ */
