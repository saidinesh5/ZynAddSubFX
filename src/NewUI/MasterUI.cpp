/*
  ZynAddSubFX - a software synthesizer

  MasterUI.cpp - The main window UI

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

#include "MasterUI.h"
#include "AddnoteUI.h"
#include "../Controls/Node.h"
#include <QtDebug>
#include <QUndoStack>
#include <QUndoView>
#include "DebugInterface.h"
#include "BankLoader.h"
#include "PropertyWatch.h"
#include "../Misc/XMLwrapper.h"

MasterUI::MasterUI(Master *master_, int *exitprogram_)
    :QMainWindow(NULL),
      Pexitprogram(exitprogram_),
      master(master_),
      m_bankUI(NULL)

{


    Q_INIT_RESOURCE(main);

    //this instance will monitor all dynamic event property changes
    new PropertyWatch(this);

    qDebug() << "Created new part at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Created new part at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Created new part at " << QString::fromStdString(
        master->parts.createChild(0));

    setupUi(this);

    //make the app-wide colorsheme the one set for this dialog
    QApplication::setPalette(palette());

    partBar->setControlsWidget(partFrame);
    partBar->setNode(Node::get("MASTER.PARTS"));

    (new DebugInterface(NULL, master))->show();

    /*
        //testcode for experimental recursive xml loading
    XMLwrapper file;
    master->saveXml(&file);
    //qDebug() << file.getXMLdata();
    file.saveXMLfile("/tmp/inst.xml");
    */

    QUndoStack *undoStack = new QUndoStack(this);
    ControlHelper::setGlobalUndoStack(undoStack);
    QUndoView *undoView = new QUndoView(undoStack, NULL);
    undoView->show();

}

void MasterUI::refresh_master_ui()
{
    //TODO: do something here! right now this is only here because it is called from main.cpp
    //in the original zyn sources
}

void MasterUI::on_editInstrument_clicked()
{
    QString id = partBar->getCurrentChild() + ".INSTRUMENT.INSTRUMENT_KIT.ADnoteParameters";
    QWidget *w;

    if(!id.isEmpty())
        (w = new AddnoteUI(id))->show();
}

void MasterUI::on_action_Quit_triggered()
{
    QCoreApplication::instance()->quit();

    //TODO: this way of exiting does not work cleanly yet
    *Pexitprogram = 1;
}

void MasterUI::on_loadInstrumentButton_clicked()
{
    QString id = partBar->getCurrentChild();
    BankLoader *loaderPopup = new BankLoader(id);
    loaderPopup->show();
}

#include "MasterUI.moc"

