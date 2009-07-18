#include <QMainWindow>
#include "../Misc/Master.h"
#include "ui_masterUI.h"

#ifndef _MASTERUI_H_
#define _MASTERUI_H_

class MasterUI : public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

public:
    MasterUI(Master *master_,int *exitprogram_);
    void refresh_master_ui();

private slots:
    void on_action_Quit_triggered();
    void on_partSelector_valueChanged(int value);
    void on_editInstrument_clicked();
private:

    int *Pexitprogram;
    Master *master;

    class BankUI *m_bankUI;
};


#endif /* include _MASTERUI_H_ */
