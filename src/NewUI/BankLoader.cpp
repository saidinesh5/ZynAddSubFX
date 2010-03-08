/*
  ZynAddSubFX - a software synthesizer

  BankLoader.cpp - An ui for loading an instrument

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

#include "BankLoader.h"
#include "ControlHelper.h"

BankLoader::BankLoader(QString partId, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    ControlHelper *instrumentHelper = new ControlHelper(instrumentList);

    connect(instrumentHelper, SIGNAL(optionsChanged(QStringList)),
            this, SLOT(slotBankChanged(QStringList)));

    connect(instrumentList, SIGNAL(currentRowChanged(int)),
            instrumentHelper, SLOT(setValue(int)));


    setProperty("absoluteControlId", partId);
    setWindowTitle(partId.split(".").last());
}

void BankLoader::slotBankChanged(QStringList instruments)
{
    instrumentList->clear();
    instrumentList->addItems(instruments);
}

#include "BankLoader.moc"
// vim: sw=4 sts=4 et tw=100
