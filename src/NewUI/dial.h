#ifndef _DIAL_H_
#define _DIAL_H_

#include <QDial>
#include "../globals.h"
#include "controlhelper.h"

class Dial:public QDial
{
    Q_OBJECT
    public:
        Dial(QWidget *parent);
        void setControl(GenControl *control);

    private slots:
        void slotUpdateSource();

    private:
        void mousePressEvent(class QMouseEvent * event);
        void mouseReleaseEvent(class QMouseEvent * event);
        void mouseMoveEvent(class QMouseEvent * event);
        void wheelEvent(class QWheelEvent * event);
        void paintEvent(class QPaintEvent * event);

        int m_originalMouseY;
        int m_originalValueOnPress;
};

#endif /* #ifndef _DIAL_H_ */

