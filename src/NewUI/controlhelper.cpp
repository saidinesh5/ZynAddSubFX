#include "controlhelper.h"
#include <QDynamicPropertyChangeEvent>
#include <QVariant>
#include <QtDebug>


//TODO: this class should also remove itself from the control its
//connected to, whenever its deleted

ControlHelper::ControlHelper(QObject *parent)
        : QObject(parent),
        m_control(NULL),
        expectedValueEvents(0)
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

void ControlHelper::handleEvent(Event *event)
{
    //will be called from the engine thread, so be careful (threadwise)
    //and efficient about all processing in this function

    if (event->type() == Event::NewValueEvent) {

        //the logic here is: if we are expecting an event about a new value, then we will ignore it
        //to avoid infinite loops
        expectedEventMutex.lock();
        if (expectedValueEvents > 0) {
            expectedValueEvents--;
            expectedEventMutex.unlock();
            return;
        }
        expectedEventMutex.unlock();

        emit valueChanged(static_cast<NewValueEvent*>(event)->val);
    } else if (event->type() == Event::RemovalEvent) {
        
    }
}

void ControlHelper::setControl(QString absoluteId)
{

    if (absoluteId.isEmpty()) {
        if (m_control) {
            m_control->removeRedirections(this);
            m_control = NULL;
        }
        return;
    }

    Node *node = Node::find(absoluteId.toStdString());
    m_control = dynamic_cast<GenControl*>(node);
    if (m_control) {
        expectedValueEvents = 0;
        m_control->addRedirection(this);
        qDebug() << "Assigning " << this << " to " << absoluteId;
        m_control->requestValue();
    } else
        qDebug() << "Could not find a control named " << absoluteId;
}

void ControlHelper::setValue(char value)
{
    if (m_control) {
        m_control->setValue(value);

        expectedEventMutex.lock();
        expectedValueEvents++;
        expectedEventMutex.unlock();
    }
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

void ControlHelper::MIDILearn()
{
    if (m_control) {
        bool result = m_control->MIDILearn();
    }
}

#include "controlhelper.moc"
