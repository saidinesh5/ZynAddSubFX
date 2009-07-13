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

void ControlHelper::handleEvent(Event &event)
{
    if (event.type() == Event::NewValueEvent) {
        qDebug() << "New value " << int(static_cast<NewValueEvent&>(event).val);
        emit valueChanged(static_cast<NewValueEvent&>(event).val);
    }
}

void ControlHelper::setControl(QString absoluteId)
{
    Node *node = Node::find(absoluteId.toStdString());
    m_control = dynamic_cast<GenControl*>(node);
    if (m_control) {
        m_control->addRedirection(this);
        qDebug() << "Assigning " << this << " to " << absoluteId;
        m_control->requestValue();
    } else
        qDebug() << "Could not find a control named " << absoluteId;
}

void ControlHelper::setValue(char value)
{
    qDebug() << "set to " << (int)value << " but control: " << m_control;
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

void ControlHelper::requestValue()
{
    if (m_control) {
        m_control->requestValue();
    }
}

#include "controlhelper.moc"
