#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QWidget>
#include "ArrayControlHelper.h"
#include "../Controls/ArrayControl.h"

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
