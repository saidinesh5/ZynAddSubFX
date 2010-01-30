#include "SpinBox.h"
#include "ControlHelper.h"
#include "../Controls/DescRanger.h"

SpinBox::SpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(valueChanged(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));

    connect(helper, SIGNAL(connected(GenControl *)),
            this, SLOT(connected(GenControl *)));
}

void SpinBox::connected(class GenControl *control)
{
    if (DescRanger* desc = dynamic_cast<DescRanger*>(control)) {
        setMinimum(desc->getMin());
        setMaximum(desc->getMax());
    }
}

#include "SpinBox.moc"
// vim: sw=4 sts=4 et tw=100
