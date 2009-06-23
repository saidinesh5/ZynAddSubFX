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
            setControl(newControl);
            return true;
        }
        if (ev->propertyName() == "relativeControlId") {
            m_relativeControlId = watched->property("relativeControlId").toString();
            return true;
        }
    }
    return false;
}

void ControlHelper::setControl(QString absoluteId)
{
    m_control = ControlContainer::find(absoluteId.toStdString());
    if (m_control) {
        m_control->registerUser(this);
        qDebug() << "Assigning " << this << " to " << absoluteId;
        emit valueChanged(m_control->getValue());
    } else
        qDebug() << "Could not find a control named " << absoluteId;
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

QString ControlHelper::getControlId()
{
    if (m_control)
        return QString::fromStdString(m_control->getAbsoluteId());
    else
        return "Undefined";
}

QString ControlHelper::relativeControlId() const
{
    return m_relativeControlId;
}


#include "controlhelper.moc"
