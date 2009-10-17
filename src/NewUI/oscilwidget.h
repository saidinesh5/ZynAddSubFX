#ifndef OSCILWIDGET_H
#define OSCILWIDGET_H

#include <QWidget>
#include "controlhelper.h"

class OscilWidget : public QWidget
{
    Q_OBJECT

    public:
        OscilWidget(QWidget *parent = NULL);

    private slots:
        void readArray(ArrayControl* array);

};

#endif // OSCILWIDGET_H
