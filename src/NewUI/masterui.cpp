#include "masterui.h"
#include "bankui.h"
#include "addnoteui.h"
#include "voicelist.h"
#include "../Controls/Node.h"
#include "eventhelper.h"
#include <QtDebug>
#include "debuginterface.h"

MasterUI::MasterUI(Master *master_,int *exitprogram_)
        : QMainWindow(NULL),
        Pexitprogram(exitprogram_),
        master(master_),
        m_bankUI(NULL)

{
    //this instance will monitor all dynamic event property changes
    new ControlHelper(QCoreApplication::instance());

    qDebug() << "Trying to find control Master.Volume";
    //qDebug() << "Returned " << Node::getRoot()->findControl("Master.Volume");

    qDebug() << "Got new child at " << QString::fromStdString(master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->parts.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->parts.createChild(0));

    setupUi(this);
    EventHelper::getInstance();

    partBar->setControlsWidget(partFrame);
    partBar->setNode(Node::find("Master.Parts"));

    Node::getRoot()->printTree();

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
    m_bankUI = new BankUI(NULL, master, new int(0));
    m_bankUI->setProperty("absoluteControlId", partBar->getCurrentChild());
    m_bankUI->show();
    return;
    QWidget *w = new VoiceList(partBar->getCurrentChild() + ".Instrument.InstrumentKit.ADnoteParameters.Voices");
    w->show();

    return;
    QString id = partBar->getCurrentChild();
    if (!id.isEmpty()) (new AddNoteUi(id))->show();
}

void MasterUI::on_action_Quit_triggered()
{
    *Pexitprogram = 1;
}
#include "masterui.moc"
