/*
  ZynAddSubFX - a software synthesizer

  BankLoader.h - An ui for loading an instrument

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
