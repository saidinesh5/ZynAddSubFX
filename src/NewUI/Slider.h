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

    private:
        bool m_outside, m_sliding;
        int m_prevValue;
};

#endif // SLIDER_H
