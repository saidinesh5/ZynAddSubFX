#include "CheckBox.h"
#include "ControlHelper.h"

CheckBox::CheckBox(QWidget *parent)
    :QCheckBox(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(clicked(bool)),
            helper, SLOT(setValue(bool)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setChecked(int)));

    QAbstractButton::setChecked((bool)helper->getValue());
}

void CheckBox::setChecked(int val)
{
    QAbstractButton::setChecked(bool(val));
}

#include "CheckBox.moc"

// vim: sw=4 sts=4 et tw=100

