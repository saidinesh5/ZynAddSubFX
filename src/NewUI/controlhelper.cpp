#include "controlhelper.h"
#include <QCoreApplication>
#include <QDynamicPropertyChangeEvent>
#include <QStack>
#include <QVariant>
#include <QtDebug>


//TODO: this class should also remove itself from the control its
//connected to, whenever its deleted

ControlHelper::ControlHelper(QObject *parent)
        : QObject(parent),
        m_control(NULL),
        expectedValueEvents(0)
{
}


ControlHelper::ControlHelper(QCoreApplication *app)
    : QObject(app)
{
    app->installEventFilter(this);
}

bool ControlHelper::eventFilter ( QObject * object, QEvent * event )
{
    //remember that this functions is called for _all_ events

    if (event->type() == QEvent::QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *ev =
            static_cast<QDynamicPropertyChangeEvent*>(event);
        if ((ev->propertyName() == "absoluteControlId") ||
                (ev->propertyName() == "controlId")) {
            QList<ControlHelper*> controlHelpers = object->findChildren<ControlHelper*>();
            foreach (ControlHelper* helper, controlHelpers) {
                helper->updateControlId();
            }
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
    if (m_control) {
        m_control->removeRedirections(this);
        m_control = NULL;
    }

    if (absoluteId.isEmpty()) {
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

void ControlHelper::updateControlId()
{
    QString fullid = parent()->property("absoluteControlId").toString();

    if (!fullid.isEmpty()) {
        //alright, we already have what we wanted
        setControl(fullid);
        return;
    }

    QObject *p = parent(); //this is the object that created the controlhelper
    fullid = p->property("controlId").toString();
    if (fullid.isEmpty()) {
        //the parent has no controlId set, so theres no reason to recurse any further for what its
        //full path would be.
        return;
    }

    while (true) {
        p = p->parent();
        if (!p) {
            //we've reached the end of the hierarchy without finding any absolute id. bail out
            return;
        }

        QString id = p->property("controlId").toString();
        QString absid = p->property("absoluteControlId").toString();

        if (!id.isEmpty()) {
            //just append the relative id and continue recursion
            fullid.prepend(id + ".");
        } else if (!absid.isEmpty()) {
            //this appears to be the absolute id we've been looking for.
            fullid.prepend(absid + ".");
            setControl(fullid);
            return;
        } 
    }
}

#include "controlhelper.moc"
