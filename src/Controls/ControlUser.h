#ifndef _CONTROLUSER_H_
#define _CONTROLUSER_H_

#include "Control.h"

class ControlUser
{
	public:
		virtual void controlChanged(Control* control) = 0;
};

#endif /* ifndef _CONTROLUSER_H_ */
