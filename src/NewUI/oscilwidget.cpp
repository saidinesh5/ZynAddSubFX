#include "oscilwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>

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

    float max = -999;
    for (int i = 0; i < OSCIL_SIZE; ++i) {
        if (m_data[i] > max) {
             max = m_data[i];
        }
    }
    max = 1 / max;

    float multiplier = float(OSCIL_SIZE) / width();

    for (int x = 0; x < width(); ++x) {
        p.drawLine(
                x, 0,
                x, height() - height() * m_data[int(x*multiplier)] * max);
    }
}

#include "oscilwidget.moc"
// vim: sw=4 sts=4 et tw=100
