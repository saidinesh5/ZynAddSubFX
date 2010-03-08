/*
  ZynAddSubFX - a software synthesizer

  MasterUI.h - The main window UI

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include <QMainWindow>
#include "../Misc/Master.h"
#include "ui_MasterUI.h"

#ifndef _MASTERUI_H_
#define _MASTERUI_H_

class MasterUI:public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

    public:
        MasterUI(Master *master_, int *exitprogram_);
        void refresh_master_ui();

    private slots:
        void on_action_Quit_triggered();
        void on_editInstrument_clicked();
        void on_buttonControllers_clicked();
        void on_loadInstrumentButton_clicked();
    private:

        int    *Pexitprogram;
        Master *master;

        class BankUI * m_bankUI;
};


#endif /* include _MASTERUI_H_ */

