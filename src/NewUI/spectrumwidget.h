#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QWidget>
#include "controlhelper.h"

class SpectrumWidget : public QWidget
{
    Q_OBJECT

    public:
        SpectrumWidget(QWidget *parent = NULL);
        void paintEvent(class QPaintEvent* event);

    private slots:
        void readArray(ArrayControl* array);

    private:
        REALTYPE *m_data;
        int m_size;

};

#endif // SPECTRUMWIDGET_H
