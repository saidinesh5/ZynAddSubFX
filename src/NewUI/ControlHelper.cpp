#include "ControlHelper.h"
#include <QCoreApplication>
#include <QWidget>
#include <QDynamicPropertyChangeEvent>
#include <QStack>
#include <QVariant>
#include <QtDebug>
#include "Menu.h"

ControlHelper::ControlHelper(QObject *parent)
    :QObject(parent),
      m_control(NULL)
{
    if (QWidget* widget = qobject_cast<QWidget*>(parent)) {
        new Menu(widget, this);
    }
}

ControlHelper::~ControlHelper()
{
    if (m_control)
        m_control->removeRedirections(this);
}

ControlHelper::ControlHelper(QCoreApplication *app)
    :QObject(app)
{
    app->installEventFilter(this);
}

bool ControlHelper::eventFilter(QObject *object, QEvent *event)
{
    //remember that this functions is called for _all_ events

    if(event->type() == QEvent::QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *ev =
            static_cast<QDynamicPropertyChangeEvent *>(event);
        if((ev->propertyName() == "absoluteControlId")
           || (ev->propertyName() == "controlId")) {
            QList<ControlHelper *> controlHelpers =
                object->findChildren<ControlHelper *>();
            foreach(ControlHelper * helper, controlHelpers)
            {
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

    if(event->type() == Event::NewValueEvent) {

        if (static_cast<NewValueEvent *>(event)->control != m_control) {
            qDebug() << "ControlHelper: Weird, events from a control we don't know?";
            return;
        }
        emit valueChanged(getValue());

        if (ArrayControl *arr_control = dynamic_cast<ArrayControl*>(m_control)) {
            emit arrayUpdated(arr_control);
        }
    }
    else
    if(event->type() == Event::RemovalEvent) {
        //clear the current connected control
        disconnect();
        qDebug() << "Got removalevent, clearing...";
    }
    else
    if(event->type() == Event::OptionsChangedEvent)
        emitOptions();
}

void ControlHelper::setControl(QString absoluteId)
{
    if (m_control) m_control->removeRedirections(this);
    disconnect();

    if(absoluteId.isEmpty())
        return;

    Node::lock();
    Node *node = Node::get(absoluteId.toStdString());
    m_control = dynamic_cast<GenControl *>(node);
    if(m_control) {
        m_control->addRedirection(this);

        emit connected(m_control);
        emitOptions();
        qDebug() << "Assigning " << this << " to " << absoluteId;
        emit valueChanged(getValue());

        ArrayControl *arr_control = dynamic_cast<ArrayControl*>(m_control);
        if (arr_control) {
            emit arrayUpdated(arr_control);
        }
        //m_control->requestValue();
    }
    else
        qDebug() << "Could not find a control named " << absoluteId;

    Node::unlock();
}

void ControlHelper::disconnect()
{
    if(m_control) {
        m_control = NULL;
    }
    emit disconnected();
}

void ControlHelper::setValue(int value)
{
    if (Control<int>* intc = dynamic_cast<Control<int>*>(m_control)) {
        intc->setValue(value);
    }
    else if (m_control) {
        m_control->setChar(value);
    }
}

void ControlHelper::setValue(bool value)
{
    setValue(int(127 * value));
}

QString ControlHelper::getControlId()
{
    if(m_control)
        return QString::fromStdString(m_control->getAbsoluteId());
    else
        return "Undefined";
}

int ControlHelper::getValue()
{
    if (Control<int>* intc = dynamic_cast<Control<int>*>(m_control)) {
        return intc->getValue();
    }
    if(m_control) {
        return m_control->getChar();
    }

    qDebug() << "Error: value for nonconnected control requested";
    return 64;
}

void ControlHelper::MIDILearn()
{
    if(m_control)
        bool result = m_control->MIDILearn();
}

void ControlHelper::trigger()
{
    setValue(127);
}

void ControlHelper::updateControlId()
{
    QString fullid = findComponentPath(parent());
    if(fullid.isEmpty())
        return;
    setControl(fullid);
}

QString ControlHelper::findComponentPath(QObject *object)
{
    if (!object) return QString();

    QString fullid = object->property("absoluteControlId").toString();

    if(!fullid.isEmpty())
        //alright, we already have what we wanted
        return fullid;

    QObject *p = object;
    fullid = p->property("controlId").toString();
    if(fullid.isEmpty())
        //the object has no controlId set, so theres no reason to recurse any further for what its
        //full path would be.
        return QString();

    while(true) {
        p = p->parent();
        if(!p)
            //we've reached the end of the hierarchy without finding any absolute id. bail out
            return QString();

        QString id    = p->property("controlId").toString();
        QString absid = p->property("absoluteControlId").toString();

        if(!id.isEmpty())
            //just append the relative id and continue recursion
            fullid.prepend(id + ".");
        else
        if(!absid.isEmpty()) {
            //this appears to be the absolute id we've been looking for.
            fullid.prepend(absid + ".");
            return fullid;
        }
    }
    return QString();
}

void ControlHelper::emitOptions()
{
    QStringList options;
    if(m_control && m_control->numOptions())
        for(int i = 0; i < m_control->numOptions(); ++i)
            options << QString::fromStdString(m_control->getOption(i));
    //qDebug() << "Emitting options " << options;
    emit optionsChanged(options);
}

void ControlHelper::defaults()
{
    if (m_control) {
        m_control->defaults();
    }
}

#include "ControlHelper.moc"

