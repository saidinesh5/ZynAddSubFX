#ifndef OSCIL_H
#define OSCIL_H

#include "ui_Oscil.h"
#include <QDialog>

class Oscil : public QDialog, public Ui::OscilUi
{
    Q_OBJECT

    public:
        Oscil(QWidget *parent=NULL);

};

#endif // OSCIL_H
