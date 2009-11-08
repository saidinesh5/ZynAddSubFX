#ifndef OSCILWIDGET_H
#define OSCILWIDGET_H

#include <QWidget>
#include "controlhelper.h"

class OscilWidget : public QWidget
{
    Q_OBJECT

    public:
        OscilWidget(QWidget *parent = NULL);
        void paintEvent(class QPaintEvent* event);

    private slots:
        void readArray(ArrayControl* array);

    private:
        REALTYPE *m_data;
        int m_size;

};

#endif // OSCILWIDGET_H
