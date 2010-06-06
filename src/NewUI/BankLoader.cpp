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
#include <QTableWidgetItem>
#include <QtDebug>

static const int colsize = 16;

BankLoader::BankLoader(QString partId, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    ControlHelper *instrumentHelper = new ControlHelper(instrumentTable);

    connect(instrumentHelper, SIGNAL(optionsChanged(QStringList)),
            this, SLOT(slotBankChanged(QStringList)));

    connect(instrumentTable, SIGNAL(currentCellChanged(int , int , int , int)),
            this, SLOT(slotCurrentCellChanged(int, int, int, int)));

    connect(this, SIGNAL(currentInstrumentChanged(int)),
            instrumentHelper, SLOT(setValue(int)));


    setProperty("absoluteControlId", partId);
    setWindowTitle(partId.split(".").last());

    for (int i = 0; i < instrumentTable->horizontalHeader()->count(); ++i) {
        instrumentTable->horizontalHeader()->setResizeMode(i, QHeaderView::Stretch);
    }

    for (int i = 0; i < instrumentTable->verticalHeader()->count(); ++i) {
        instrumentTable->verticalHeader()->setResizeMode(i, QHeaderView::Stretch);
    }

}

void BankLoader::slotBankChanged(QStringList instruments)
{
    instrumentTable->clearContents();

    for (int i = 0; i < instruments.size(); ++i) {
        instrumentTable->setItem(i % colsize, i / colsize, new QTableWidgetItem(instruments.at(i)));
    }
    //instrumentList->addItems(instruments);
}

void BankLoader::slotCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    int instrumentIndex = currentColumn * colsize + currentRow;

    if (loadOnSelectChange->isChecked()) {
        emit currentInstrumentChanged(instrumentIndex);
    }
}

#include "BankLoader.moc"
// vim: sw=4 sts=4 et tw=100
