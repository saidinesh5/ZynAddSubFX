#ifndef OSCIL_H
#define OSCIL_H

#include "ui_oscil.h"
#include <QDialog>

class Oscil : public QDialog, public Ui::OscilUi
{
    Q_OBJECT

    public:
        Oscil(QWidget *parent=NULL);

};

#endif // OSCIL_H
