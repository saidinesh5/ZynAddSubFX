#include "masterui.h"
#include "bankui.h"
#include "addnoteui.h"
#include "../Controls/Node.h"
#include "eventhelper.h"
#include <QtDebug>

MasterUI::MasterUI(Master *master_,int *exitprogram_)
        : QMainWindow(NULL),
        Pexitprogram(exitprogram_),
        master(master_),
        m_bankUI(NULL)

{
    qDebug() << "Trying to find control Master.Volume";
    //qDebug() << "Returned " << Node::getRoot()->findControl("Master.Volume");

    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createChild(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createChild(0));

    setupUi(this);
    EventHelper::getInstance();

    partBar->addControlWidgets(partFrame);
    partBar->setNode(Node::find("Master.Parts"));

    Node::getRoot()->printTree();
}

void MasterUI::refresh_master_ui()
{
    //TODO: do something here! right now this is only here because it is called from main.C
}

void MasterUI::on_partSelector_valueChanged(int value)
{
    //npart = value;
}

void MasterUI::on_action_Quit_triggered()
{
    *Pexitprogram = 1;
}
#include "masterui.moc"
