#include "masterui.h"
#include "bankui.h"
#include "addnoteui.h"
#include "../Controls/ControlContainer.h"
#include "eventhelper.h"
#include <QtDebug>

MasterUI::MasterUI(Master *master_,int *exitprogram_)
        : QMainWindow(NULL),
        Pexitprogram(exitprogram_),
        master(master_),
        m_bankUI(NULL)

{
    qDebug() << "Trying to find control Master.Volume";
    qDebug() << "Returned " << ControlContainer::getRoot()->findControl("Master.Volume");

    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createControlContainer(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createControlContainer(0));
    qDebug() << "Got new child at " << QString::fromStdString(master->instrumentContainer.createControlContainer(0));

    setupUi(this);
    EventHelper::getInstance();

    partBar->addControlWidgets(partFrame);
    partBar->setChildrenContainer(ControlContainer::getRoot()->findContainer("Master.Parts"));

    ControlContainer::getRoot()->printTree();
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
