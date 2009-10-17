#include "oscilwidget.h"

OscilWidget::OscilWidget(QWidget *parent)
    : QWidget(parent)
{
    ControlHelper *helper = new ControlHelper(this);
    connect(helper, SIGNAL(arrayUpdated(ArrayControl*)),
            this, SLOT(readArray(ArrayControl*)));
}

void OscilWidget::readArray(ArrayControl* array)
{

}

#include "oscilwidget.moc"
// vim: sw=4 sts=4 et tw=100
