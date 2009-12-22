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

    private:
        bool m_sliding, m_outside;
        int m_prevValue;
};

#endif // SLIDER_H
