/*
  ZynAddSubFX - a software synthesizer

  Keyboard.cpp - A virtual keyboard widget

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

#include "Keyboard.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include "../Misc/Master.h"
#include <QtDebug>

static const int octaves = 5;
static const int channel = 0;
static const int rootnote = 60 - 12; //C-3

//how wide are the black ceys relative to the white ones?
static const float blackKeyMult = 0.7;

//how high are black keys relative to widget height
static const float blackKeyHeight = 0.5;

//these two arrays define the correlation between black/white tangent 0-7, and their corresponding
//midi note (relative to the start of the octave)
static const int whiteIndex[] = {
    0, 2, 4, 5, 7, 9, 11
};
static const int blackIndex[] = {
    1, 3, -1, 6, 8, 10, -1
};

Keyboard::Keyboard(QWidget *parent)
    :QWidget(parent),
    m_currentMouseNote(-1)
{

}

int Keyboard::toNote(const QPoint& pos)
{
    const float whiteKeyWidth = float(width()) / (7*octaves);
    const float blackKeyWidth = whiteKeyWidth * blackKeyMult;
    const float octaveWidth = float(width()) / octaves;

    int octave = pos.x() / octaveWidth;
    int rest = pos.x() - octave * octaveWidth;

    if (pos.x() < 0) return 0;
    if (pos.x() >= width()) return rootnote + octaves * 12;

    if (pos.y() < blackKeyHeight * height()) {
        //although there are both white and black keys on the top, we choose to say this is a black key
        int keyOffset = blackIndex[int((rest - blackKeyWidth / 2) / whiteKeyWidth)];

        //if this is between some black keys, we return the corresponding white key
        if (-1 == keyOffset) {
            keyOffset = blackIndex[int((rest - blackKeyWidth / 2) / whiteKeyWidth) - 1];
        }

        int ret = rootnote + octave * 12 + keyOffset;
        return ret;

    } else {
        //we know it's a white key
        int ret = rootnote + octave * 12 + whiteIndex[int((rest / whiteKeyWidth))];
        return ret;
    }
}

void Keyboard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::black);

    const float whiteKeyWidth = float(width()) / (7*octaves);
    const float blackKeyWidth = whiteKeyWidth * blackKeyMult;

    for (int octave = 0; octave < octaves; ++octave) {

        const int octaveStart = 12*octave;

        p.setBrush(Qt::white);
        for (int key = 0; key < 7; ++key) {
            bool keyIsPlaying = (m_currentMouseNote == rootnote + octaveStart + whiteIndex[key]);

            if (keyIsPlaying) p.setBrush(Qt::red);
            p.drawRect((7*octave + key) * whiteKeyWidth, 0, whiteKeyWidth, height());
            if (keyIsPlaying) p.setBrush(Qt::white);
        }

        //now draw black keys
        p.setBrush(Qt::black);
        for (int key = 0; key < 7; ++key) {

            if (key == 2 || key == 6) continue;

            const int blackOffset = 0.5 * whiteKeyWidth + blackKeyWidth / 4;
            bool keyIsPlaying = (m_currentMouseNote == rootnote + octaveStart + blackIndex[key]);

            if (keyIsPlaying) p.setBrush(Qt::red);
            p.drawRect((7*octave + key) * whiteKeyWidth + blackOffset, 0, blackKeyWidth, height() * blackKeyHeight);
            if (keyIsPlaying) p.setBrush(Qt::black);
        }
    }
}

void Keyboard::singleNoteOn(int note)
{
    if (note == m_currentMouseNote) return;

    Master *master = static_cast<Master*>(Node::getRoot());
    master->mutexLock(MUTEX_LOCK);

    if (-1 != m_currentMouseNote) {
        master->noteOff(channel, m_currentMouseNote);
    }

    master->noteOn(channel, note, 127);
    m_currentMouseNote = note;
    master->mutexLock(MUTEX_UNLOCK);
}

void Keyboard::singleNoteOff()
{
    if (-1 == m_currentMouseNote) return;

    Master *master = static_cast<Master*>(Node::getRoot());
    master->mutexLock(MUTEX_LOCK);

    master->noteOff(channel, m_currentMouseNote);

    master->mutexLock(MUTEX_UNLOCK);

    m_currentMouseNote = -1;
}

void Keyboard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    int note = toNote(event->pos());
    singleNoteOn(note);
    update();
}

void Keyboard::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    int note = toNote(event->pos());
    singleNoteOn(note);
    update();
}


void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    singleNoteOff();
    update();
}

#include "Keyboard.moc"
// vim: sw=4 sts=4 et tw=100
