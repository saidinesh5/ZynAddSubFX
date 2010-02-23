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

        class Master *m_master;
        int m_currentMouseNote;

};

#endif // KEYBOARD_H
