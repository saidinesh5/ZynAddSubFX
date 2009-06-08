#ifndef _CONTROLHELPER_H_
#define _CONTROLHELPER_H_

#include "../Controls/Control.h"
#include "../Controls/ControlUser.h"
#include <QObject>

class ControlHelper : public QObject, public ControlUser
{
	Q_OBJECT
	public:
		ControlHelper(QObject *parent, Control *control);
		void controlChanged(Control* control);


	private:
		Control *m_control;
};

#endif /* ifndef _CONTROLHELPER_H_ */
