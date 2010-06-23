/*
  ZynAddSubFX - a software synthesizer

  Dial.h - A dial widget

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

#ifndef _DIAL_H_
#define _DIAL_H_

#include <QDial>
#include "../globals.h"
#include "ControlHelper.h"

class Dial:public QDial
{
    Q_OBJECT
    public:
        Dial(QWidget *parent);
        //QSize minimumSizeHint() const;
        //QSize sizeHint() const;

    private slots:
        void slotConnected(GenControl *control);
        void slotDisconnected();
        void slotIncomingValue(int value);

    private:
        void mousePressEvent(class QMouseEvent * event);
        void mouseReleaseEvent(class QMouseEvent * event);
        void mouseMoveEvent(class QMouseEvent * event);
        void wheelEvent(class QWheelEvent * event);
        void paintEvent(class QPaintEvent * event);

        void cleanUpString(QString& string);

        int m_originalMouseY;
        int m_originalValueOnPress;
        bool m_isConnected;
};

#endif /* #ifndef _DIAL_H_ */

