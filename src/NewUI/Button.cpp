#include "Button.h"
#include "ControlHelper.h"
#include <QtDebug>

Button::Button(QWidget *parent)
    : QPushButton(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(clicked()),
            helper, SLOT(trigger()));
}


#include "Button.moc"
// vim: sw=4 sts=4 et tw=100
