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
    memset(m_data, 0, sizeof(REALTYPE)*OSCIL_SIZE);
    m_size = OSCIL_SIZE;
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

    REALTYPE barwidth = REALTYPE(width()) / (m_size);

    p.setPen(palette().color(QPalette::Text));

    int prev;

    REALTYPE max = -999;
    for (int i = 0; i < m_size; ++i) {
        if (fabs(m_data[i]) > max) {
            max = fabs(m_data[i]);
        }
    }
    max=max*1.05;

    //draws the spectrum
    for (int i=0;i<m_size;i++){
        REALTYPE x=m_data[i];
        int val=height() * 0.5 - int(0.5 * height() * (x/max));

        if (0 == i) {
            prev = val;
        }

        p.drawLine(
                i * barwidth, val,
                (i - 1)*barwidth, prev);

        prev = val;

    }
}

#include "oscilwidget.moc"
// vim: sw=4 sts=4 et tw=100
