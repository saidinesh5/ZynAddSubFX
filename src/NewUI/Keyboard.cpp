#include "Keyboard.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include "../Misc/Master.h"
#include <QtDebug>

static const int octaves = 3;
static const int channel = 0;
static const int rootnote = 12*3;
static const int tangents = octaves * 12;

Keyboard::Keyboard(QWidget *parent)
    :QWidget(parent),
    m_currentMouseNote(-1)
{

}

int Keyboard::toNote(const QPoint& pos)
{
    int tangentWidth = width() / tangents;

    return rootnote + pos.x() / tangentWidth;
}

void Keyboard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    int note = toNote(event->pos());

    Master *master = static_cast<Master*>(Node::getRoot());
    qDebug() << "Noteon";
    master->mutexLock(MUTEX_LOCK);

    if (-1 != m_currentMouseNote) {
        qDebug() << "Noteoff";
        master->noteOff(channel, m_currentMouseNote);
    }
    master->noteOn(channel, note, 127);
    m_currentMouseNote = note;
    master->mutexLock(MUTEX_UNLOCK);

    update();
}

void Keyboard::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    int note = toNote(event->pos());

    if (note == m_currentMouseNote) return;

    Master *master = static_cast<Master*>(Node::getRoot());
    master->mutexLock(MUTEX_LOCK);

    if (-1 != m_currentMouseNote) {
        qDebug() << "Noteoff";
        master->noteOff(channel, m_currentMouseNote);
    }
    qDebug() << "Noteon";
    master->noteOn(channel, note, 127);
    m_currentMouseNote = note;
    master->mutexLock(MUTEX_UNLOCK);

    update();
}

void Keyboard::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    int tangentWidth = width() / tangents;

    for (int i = 0; i < tangents; ++i) {
        if (i != (m_currentMouseNote - rootnote)) {
            p.drawRect(i*tangentWidth, 0, tangentWidth, height() / 2);
        } else {
            p.setBrush(Qt::black);
            p.fillRect((m_currentMouseNote - rootnote)*tangentWidth, 0, tangentWidth, height() / 2, Qt::SolidPattern);
            p.setBrush(QBrush());
        }
    }
}

void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    if (-1 != m_currentMouseNote) {
        Master *master = static_cast<Master*>(Node::getRoot());
        qDebug() << "Noteoff";
        master->mutexLock(MUTEX_LOCK);
        master->noteOff(channel, m_currentMouseNote);
        master->mutexLock(MUTEX_UNLOCK);
        m_currentMouseNote = -1;
    }
    update();
}

#include "Keyboard.moc"
// vim: sw=4 sts=4 et tw=100
