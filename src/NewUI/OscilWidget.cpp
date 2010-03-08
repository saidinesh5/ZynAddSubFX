/*
  ZynAddSubFX - a software synthesizer

  OscilWidget.cpp - A widget for drawing oscillator

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

#include "OscilWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <math.h>
#include "../globals.h"

OscilWidget::OscilWidget(QWidget *parent)
    : QWidget(parent)
{
    ControlHelper *helper = new ArrayControlHelper(this);
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

    p.setRenderHint(QPainter::Antialiasing);

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
        float val=height() * 0.5 - (0.5 * height() * (x/max));

        if (0 == i) {
            prev = val;
        }

        p.drawLine(
                QPointF(i * barwidth, val),
                QPointF((i - 1)*barwidth, prev));

        prev = val;

    }
}

#include "OscilWidget.moc"
// vim: sw=4 sts=4 et tw=100
