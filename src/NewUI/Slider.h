/*
  ZynAddSubFX - a software synthesizer

  Slider.h - A slider widget, can be both vertical and horizontal

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

#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
    public:
        Slider(QWidget *parent=NULL);
        void mousePressEvent(class QMouseEvent * event);
        void mouseReleaseEvent(class QMouseEvent * event);
        void mouseMoveEvent(class QMouseEvent * event);
        void slideEvent(QMouseEvent *event);

    signals:
        void defaults();

    private slots:
        void connected(class GenControl *control);
        void slotIncomingValue(int value);

    private:
        bool m_outside;
        int m_prevValue;
};

#endif // SLIDER_H
