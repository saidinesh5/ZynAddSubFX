#include "slider.h"
#include "controlhelper.h"
#include <QtDebug>
#include <QMouseEvent>
#include <QStyleOptionComplex>

Slider::Slider(QWidget *parent)
    : QSlider(parent),
    m_sliding(false),
    m_outside(false),
    m_prevValue(0)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(sliderMoved(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));

}

void Slider::mousePressEvent(QMouseEvent *e)
{
    QStyleOptionComplex complex;
    QRect rect  = style()->subControlRect( QStyle::CC_Slider, &complex , QStyle::SC_SliderHandle, this );
    m_sliding   = true;
    m_prevValue = QSlider::value();

    if ( !rect.contains( e->pos() ) )
        mouseMoveEvent( e );
}

void Slider::mouseReleaseEvent(QMouseEvent *event)
{
    m_sliding = false;
    m_outside = false;
}

void Slider::mouseMoveEvent(QMouseEvent *e)
{
    if ( m_sliding )
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

#include "slider.moc"
// vim: sw=4 sts=4 et tw=100
