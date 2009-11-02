#include "oscilwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <math.h>
#include "../globals.h"

OscilWidget::OscilWidget(QWidget *parent)
    : QWidget(parent)
{
    ControlHelper *helper = new ControlHelper(this);
    connect(helper, SIGNAL(arrayUpdated(ArrayControl*)),
            this, SLOT(readArray(ArrayControl*)));

    m_data = new REALTYPE[OSCIL_SIZE];
}

void OscilWidget::readArray(ArrayControl* array)
{
    array->readArray(m_data, &m_size);
    update();
}

void OscilWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter p(this);

    const int maxdb=60;//must be multiple of 10

    REALTYPE max = -999;
    for (int i = 0; i < OSCIL_SIZE; ++i) {
        if (fabs(m_data[i]) > max) {
            max = fabs(m_data[i]);
        }
    }
    if (max<0.000001) max=1.0;
    max=max*1.05;
    max = 1 / max;

    REALTYPE barwidth = REALTYPE(width()) / (OSCIL_SIZE / 2);

    p.setBrush(Qt::SolidPattern);

    //draws the spectrum
    for (int i=0;i<OSCIL_SIZE / 2;i++){
        int tmp=i*2;
        REALTYPE x=m_data[i]*max;

        if (x>dB2rap(-maxdb)) x=rap2dB(x)/maxdb+1;
        else x=0;

        int val=(int) (height()*x);
        if (val>0) 
        p.drawRect(
                i * barwidth, height() - val,
                barwidth, val);

    }
}

#include "oscilwidget.moc"
// vim: sw=4 sts=4 et tw=100
