#include "slider.h"
#include "controlhelper.h"
#include <QtDebug>

Slider::Slider(QWidget *parent)
    : QSlider(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(sliderMoved(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));

}


#include "slider.moc"
// vim: sw=4 sts=4 et tw=100
