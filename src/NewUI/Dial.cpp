/*
  ZynAddSubFX - a software synthesizer

  Dial.cpp - A dial widget

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "Dial.h"
#include <QMenu>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QtDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QConicalGradient>
#include <math.h>

//drawstyles: 0 - piechart
//            1 - rotated dial
//            2 - rectangular
//            3 - image
static int drawStyle = 1;

Dial::Dial(QWidget *parent)
    :QDial(parent),
      m_originalMouseY(-1),
      m_isConnected(false)
{
    setMouseTracking(false);
    setMinimum(0);
    setMaximum(127);
    setPageStep(32);

    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(sliderMoved(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));

    connect(helper, SIGNAL(connected(GenControl *)),
            this, SLOT(slotConnected(GenControl *)));
    connect(helper, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));

}

void Dial::mousePressEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton)) {
        m_originalMouseY = event->y();
        m_originalValueOnPress = value();
        event->accept();
        setSliderDown(true);
        return;
    }
    QDial::mousePressEvent(event);
}

void Dial::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::RightButton)) {
        QDial::mouseReleaseEvent(event);
    }
    else {
        m_originalMouseY = -1;
        setSliderDown(false);
    }
}

void Dial::mouseMoveEvent(QMouseEvent *event)
{
    if(isSliderDown()) {
        setValue(m_originalValueOnPress
                 + float(m_originalMouseY - event->y())
                 * 0.5
                );
        event->accept();
    }
    //QDial::mouseMoveEvent(event);
}

void Dial::slotConnected(GenControl * /*control*/)
{
    m_isConnected = true;
    update();
}

void Dial::slotDisconnected()
{
    m_isConnected = false;
    update();
}

void Dial::paintEvent(class QPaintEvent * /*event */)
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    if(r.width() < r.height())
        r.setHeight(r.width());
    else
    if(r.height() < r.width())
        r.setWidth(r.height());

    r.setSize(r.size() * 0.9);
    r.moveCenter(rect().center());

    float v = (float(value()) / (maximum() - minimum()));

    if(drawStyle == 0) {
        QConicalGradient grad(r.center(), 270 - 13);
        p.setBrush(QColor(Qt::white));

        p.drawEllipse(r);

        grad.setColorAt(1, Qt::yellow);
        grad.setColorAt(0.5, QColor(255, 128, 0));
        grad.setColorAt(0, Qt::red);
        p.setBrush(grad);
        p.drawPie(r, 255 * 16, -v * 16);
        //p.drawPoint(10 *
    }
    else
    if(drawStyle == 1) {

        //leave 30 degrees space at the bottom
        const int spaceAtBottom = 30;
        const int start = 270 - spaceAtBottom / 2;
        const int span = -v*(360 - spaceAtBottom);

        //center circle
        p.setBrush(palette().button());
        p.drawEllipse(r);

        p.translate(r.center());

        for(float i = start*(2*(PI/360)); i >= (-90+spaceAtBottom/2)*(2*(PI/360)); i -= PI / 12)

            //draw lots of markers
            p.drawLine(
                r.width() * 0.4 * cos(i), -r.height() * 0.4 * sin(i),
                r.width() * 0.5 * cos(i), -r.height() * 0.5 * sin(i));



        QRect smallRect = r;
        smallRect.setSize(smallRect.size() * 0.6);
        smallRect.moveCenter(QPoint(0, 0));

        if (m_isConnected)
            p.setBrush(palette().light());
        else
            p.setBrush(palette().button());

        p.drawPie(smallRect, start * 16, span * 16);
        //p.rotate(v);

        //QPoint point = r.center();
        //point.setY(point.y() - r.height() / 2);

        //p.drawEllipse(QPoint(0, r.height() / 3), 4, 4);
    }
    else
    if(drawStyle == 2) {
        r = rect();
        p.drawRect(r);
        //p.setBrush(palette().alternateBase());
        p.setBrush(QColor(Qt::white));
        p.drawRect(r.x(), r.height() + r.y() - v * r.height(),
                   r.width(), v * r.height());
    }
    else
    if(drawStyle == 3) {

        //leave 30 degrees space at the bottom
        const int spaceAtBottom = 30;
        int rotation = -180 + spaceAtBottom / 2 + v*(360 - spaceAtBottom);

        QPixmap button(":/images/spindial.png");
        p.save();
        p.translate(r.center());
        p.rotate(rotation);
        p.drawPixmap(-r.width() / 2, -r.height() / 2, r.width(), r.height(), button);

        p.restore();

        p.drawText(r, Qt::AlignCenter, QString::number(value()));
    }
}

void Dial::wheelEvent(class QWheelEvent *event)
{
    QDial::wheelEvent(event);
    emit sliderMoved(value());
}

void Dial::resizeEvent(QResizeEvent* event)
{
    QDial::resizeEvent(event);
    setMaximumHeight(width());
}

#include "Dial.moc"

