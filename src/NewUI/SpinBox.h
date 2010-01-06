#ifndef NEWUI_SPINBOX_H
#define NEWUI_SPINBOX_H

#include <QSpinBox>

class SpinBox : public QSpinBox
{
    Q_OBJECT

    public:
        SpinBox(QWidget *parent);

    private slots:
        void connected(class GenControl *control);

};

#endif // NEWUI_SPINBOX_H
