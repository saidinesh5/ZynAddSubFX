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

    qDebug() << "Trying to find control Master.Volume";
    //qDebug() << "Returned " << Node::getRoot()->findControl("Master.Volume");

    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(
        master->parts.createChild(0));

    setupUi(this);

    partBar->setControlsWidget(partFrame);
    //Node::lock();
    partBar->setNode(Node::get("Master.Parts"));
    //Node::unlock();

    //Node::getRoot()->printTree();

    //(new DebugInterface(NULL, master))->show();
}

void MasterUI::refresh_master_ui()
{
    //TODO: do something here! right now this is only here because it is called from main.C
}

void MasterUI::on_partSelector_valueChanged(int value)
{
    //npart = value;
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
    *Pexitprogram = 1;
}
#include "MasterUI.moc"

