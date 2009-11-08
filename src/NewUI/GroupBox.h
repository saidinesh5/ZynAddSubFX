#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QGroupBox>

class GroupBox : public QGroupBox
{
    Q_OBJECT

    public:
        GroupBox(QWidget *parent = NULL);

    private slots:
        void slotReset();

};

#endif // GROUPBOX_H
