/*
  ZynAddSubFX - a software synthesizer

  Slider.cpp - A slider widget, can be both vertical and horizontal

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

#include "Slider.h"
#include "ControlHelper.h"
#include <QtDebug>
#include <QMouseEvent>
#include <QStyleOptionComplex>
#include "../Controls/DescRanger.h"

Slider::Slider(QWidget *parent)
    : QSlider(parent),
    m_outside(false),
    m_prevValue(0)
{
    ControlHelper *helper = new ControlHelper(this);

    //connect(this, SIGNAL(sliderMoved(int)),
            //helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(slotIncomingValue(int)));

    connect(this, SIGNAL(defaults()),
            helper, SLOT(defaults()));
    connect(helper, SIGNAL(connected(GenControl *)),
            this, SLOT(connected(GenControl *)));

}

void Slider::slotIncomingValue(int value)
{
    if (!isSliderDown()) {
        setValue(value);
    }
}

void Slider::connected(class GenControl *control)
{
    if (DescRanger* desc = dynamic_cast<DescRanger*>(control)) {
        setMinimum(desc->getMin());
        setMaximum(desc->getMax());
    }
}

void Slider::mousePressEvent(QMouseEvent *e)
{
    QStyleOptionComplex complex;
    QRect rect  = style()->subControlRect( QStyle::CC_Slider, &complex , QStyle::SC_SliderHandle, this );

    if ( e->button() == Qt::LeftButton &&
            !rect.contains( e->pos() ) ) {

        setSliderDown(true);
        m_prevValue = QSlider::value();
        mouseMoveEvent( e );

    } else if (e->button() == Qt::MidButton) {
        emit defaults();
    } else {
        QSlider::mousePressEvent(e);
    }
}

void Slider::mouseReleaseEvent(QMouseEvent *)
{
    setSliderDown(false);
    m_outside = false;
}

void Slider::mouseMoveEvent(QMouseEvent *e)
{
    if ( isSliderDown() )
    {
        //feels better, but using set value of 20 is bad of course
        QRect rect( -20, -20, width()+40, height()+40 );

        if ( orientation() == Qt::Horizontal && !rect.contains( e->pos() ) )
        {
            if ( !m_outside )
            {
                QSlider::setValue( m_prevValue );
                //if mouse released outside of slider, emit sliderMoved to previous value
                emit sliderMoved( m_prevValue );
            }
            m_outside = true;
        }
        else
        {
            m_outside = false;
            slideEvent( e );
            emit sliderMoved( value() );
        }
    }
    else
        QSlider::mouseMoveEvent( e );
}

void Slider::slideEvent(QMouseEvent *e)
{
    QStyleOptionComplex complex;
    QRect rect = style()->subControlRect( QStyle::CC_Slider, &complex , QStyle::SC_SliderHandle, this );

    int position;
    int span;

    if( orientation() == Qt::Horizontal )
    {
        position = e->pos().x() - rect.width() / 2;
        span = width() - rect.width();
    }
    else
    {
        span = height() - rect.height();
        position = span - e->pos().y() - rect.height() / 2;
    }

    const int val = QStyle::sliderValueFromPosition( minimum(), maximum(), position, span );
    QSlider::setValue( val );
}

#include "Slider.moc"
// vim: sw=4 sts=4 et tw=100
