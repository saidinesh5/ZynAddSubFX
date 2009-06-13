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

	private slots:
		void on_action_Quit_triggered();
		void on_partSelector_valueChanged(int value);
	private:

		int *Pexitprogram;
		Master *master;

		class BankUI *m_bankUI;
};


#endif /* include _MASTERUI_H_ */
