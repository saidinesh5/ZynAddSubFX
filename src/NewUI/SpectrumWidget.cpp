/*
  ZynAddSubFX - a software synthesizer

  SpectrumWidget.cpp - A custom widget that draws an osciallator spectrum

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

#include "SpectrumWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>
#include <math.h>
#include "../globals.h"

SpectrumWidget::SpectrumWidget(QWidget *parent)
    : QWidget(parent)
{
    ControlHelper *helper = new ArrayControlHelper(this);
    connect(helper, SIGNAL(arrayUpdated(ArrayControl*)),
            this, SLOT(readArray(ArrayControl*)));

    m_data = new REALTYPE[OSCIL_SIZE];
    memset(m_data, 0, sizeof(REALTYPE)*OSCIL_SIZE/2);
    m_size = OSCIL_SIZE/2;
}

void SpectrumWidget::readArray(ArrayControl* array)
{
    array->readArray(m_data, &m_size);
    update();
}

void SpectrumWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    if (!m_data) return;

    QPainter p(this);

    const int maxdb=60;//must be multiple of 10

    REALTYPE max = -999;
    for (int i = 0; i < m_size; ++i) {
        if (fabs(m_data[i]) > max) {
            max = fabs(m_data[i]);
        }
    }
    if (max<0.000001) max=1.0;
    max=max*1.05;
    max = 1 / max;

    REALTYPE barwidth = REALTYPE(width()) / (m_size);

    //p.setBrush(QBrush(palette().color(QPalette::Text)));
    p.setPen(palette().color(QPalette::Text));

    //draws the spectrum
    for (int i=0;i<m_size;i++){
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

#include "SpectrumWidget.moc"
// vim: sw=4 sts=4 et tw=100
