/*
  ZynAddSubFX - a software synthesizer

  Keyboard.h - A virtual keyboard widget

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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>

class Keyboard : public QWidget
{
    Q_OBJECT

    public:
        Keyboard(QWidget *parent);
        virtual void mousePressEvent(class QMouseEvent *event);
        virtual void mouseMoveEvent(class QMouseEvent *event);
        virtual void mouseReleaseEvent(class QMouseEvent *event);
        void paintEvent(class QPaintEvent *event);

    private:
        int toNote(const QPoint& p);
        void singleNoteOn(int note);
        void singleNoteOff();

        class Master *m_master;
        int m_currentMouseNote;

};

#endif // KEYBOARD_H
