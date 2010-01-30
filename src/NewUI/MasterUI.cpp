#include "MasterUI.h"
#include "BankUI.h"
#include "AddnoteUI.h"
#include "VoiceList.h"
#include "../Controls/Node.h"
#include <QtDebug>
#include "DebugInterface.h"
#include <Oscil.h>

MasterUI::MasterUI(Master *master_, int *exitprogram_)
    :QMainWindow(NULL),
      Pexitprogram(exitprogram_),
      master(master_),
      m_bankUI(NULL)

{
    Q_INIT_RESOURCE(main);

    //this instance will monitor all dynamic event property changes
    new ControlHelper(QCoreApplication::instance());

    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));

    setupUi(this);

    partBar->setControlsWidget(partFrame);
    partBar->setNode(Node::get("Master.Parts"));

    (new DebugInterface(NULL, master))->show();
}

void MasterUI::refresh_master_ui()
{
    //TODO: do something here! right now this is only here because it is called from main.cpp
    //in the original zyn sources
}

void MasterUI::on_editInstrument_clicked()
{
    //m_bankUI = new BankUI(NULL, master, new int(0));
    //m_bankUI->setProperty("absoluteControlId", partBar->getCurrentChild());
    //m_bankUI->show();
    //return;

    QString id = partBar->getCurrentChild();
    QWidget *w = new VoiceList(
        id + ".Instrument.InstrumentKit.ADnoteParameters.Voices");
    w->show();

    if(!id.isEmpty())
        (w = new AddnoteUI(id))->show();

    w->setProperty("absoluteControlId", id + ".Instrument.InstrumentKit.ADnoteParameters");
}

void MasterUI::on_buttonControllers_clicked()
{
    Oscil *oscil = new Oscil(NULL);
    oscil->setProperty("absoluteControlId",
            "Master.Parts.Part1.Instrument.InstrumentKit.ADnoteParameters.Voices.VoiceParam1.OscilSmp");
    oscil->show();
}

void MasterUI::on_action_Quit_triggered()
{
    QCoreApplication::instance()->quit();

    //TODO: this way of exiting does not work cleanly yet
    *Pexitprogram = 1;
}
#include "MasterUI.moc"

