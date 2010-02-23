#include "PropertyWatch.h"
#include <QCoreApplication>
#include "ControlHelper.h"

PropertyWatch::PropertyWatch(QObject *parent)
    :QObject(parent)
{
    QCoreApplication::instance()->installEventFilter(this);
}

bool PropertyWatch::eventFilter(QObject *object, QEvent *event)
{
    //remember that this functions is called for _all_ qt events, be efficient!

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

// vim: sw=4 sts=4 et tw=100
