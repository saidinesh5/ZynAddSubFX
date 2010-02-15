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
        void slotCopy();
        void slotPaste();

};

#endif // GROUPBOX_H
