#include "ArrayControlHelper.h"

ArrayControlHelper::ArrayControlHelper(QObject *parent)
    : ControlHelper(parent)
{

}

void ArrayControlHelper::connectedEvent()
{
    ControlHelper::connectedEvent();

    if (ArrayControl* c = static_cast<ArrayControl*>(m_control)) {
        emit arrayUpdated(c);
    }
}

void ArrayControlHelper::disconnectedEvent()
{
}

void ArrayControlHelper::newValueEvent()
{
    ControlHelper::newValueEvent();

    if (ArrayControl* c = static_cast<ArrayControl*>(m_control)) {
        emit arrayUpdated(c);
    }
}

#include "ArrayControlHelper.moc"
// vim: sw=4 sts=4 et tw=100
