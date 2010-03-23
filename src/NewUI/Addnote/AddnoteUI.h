/*
  ZynAddSubFX - a software synthesizer

  AddnoteUI.h - main addnote ui

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

#include <QDialog>
#include "../Misc/Master.h"
#include "ui_AddnoteUI.h"

#ifndef _ADDNOTEUI_H_
#define _ADDNOTEUI_H_

class AddnoteUI:public QDialog, Ui::AddnoteUI
{
    Q_OBJECT

    public:
        AddnoteUI(QString id);

    private slots:
        void on_buttonShowVoicelist_clicked();

    private:
        QString m_id;
};


#endif /* include _ADDNOTEUI_H_ */

