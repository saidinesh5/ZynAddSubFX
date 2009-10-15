#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
    public:
        Slider(QWidget *parent=NULL);
};

#endif // SLIDER_H
