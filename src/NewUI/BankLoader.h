#ifndef BANKLOADER_H
#define BANKLOADER_H

#include "ui_BankLoader.h"

class BankLoader : public QWidget, public Ui::BankLoader
{
    Q_OBJECT
    public:
        BankLoader(QString partId, QWidget *parent = NULL);

    private slots:
        void slotBankChanged(QStringList instruments);

};

#endif // BANKLOADER_H
