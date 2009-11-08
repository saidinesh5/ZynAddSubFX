#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

class ComboBox:public QComboBox
{
    Q_OBJECT
    public:
        ComboBox(QWidget *parent = NULL);

    private slots:
        void setOptions(QStringList options);
};

#endif // COMBOBOX_H

