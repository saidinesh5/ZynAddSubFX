#include "controlhelper.h"
#include <QDynamicPropertyChangeEvent>
#include <QVariant>
#include <QtDebug>


//TODO: this class should also remove itself from the control its
//connected to, whenever its deleted

ControlHelper::ControlHelper(QObject *parent)
	: QObject(parent),
	m_control(NULL)
{
	parent->installEventFilter(this);
}

bool ControlHelper::eventFilter ( QObject * watched, QEvent * event )
{
	if (event->type() == QEvent::QEvent::DynamicPropertyChange) {
		QDynamicPropertyChangeEvent *ev = 
			(QDynamicPropertyChangeEvent*)event;
		Q_ASSERT(ev);
		if (ev->propertyName() == "controlId") {
			QString newControl = watched->property("controlId").toString();
			m_control = ControlContainer::find(newControl.toStdString());
			if (m_control) {
				m_control->registerUser(this);
				qDebug() << "Assigning " << this << " to " << newControl;
				emit valueChanged(m_control->getValue());
			}
			else
				qDebug() << "Could not find a control named " << newControl;
			return true;
		}
		//TODO: didn't find the control. perhaps do something?
	} 
	return false;
}

void ControlHelper::controlChanged(Control* control)
{
	emit valueChanged(control->getValue());
}

void ControlHelper::setValue(char value)
{
	qDebug() << "set to " << (int)value;
	if (m_control)
		m_control->setValue(value);
}

void ControlHelper::setValue(int value)
{
	setValue(char(qBound(0, value, 127)));
}

#include "controlhelper.moc"
