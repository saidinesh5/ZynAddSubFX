#include "masterui.h"
#include "bankui.h"
#include "addnoteui.h"
#include "../Controls/ControlContainer.h"
#include <QtDebug>

MasterUI::MasterUI(Master *master_,int *exitprogram_)
	: QMainWindow(NULL),
	Pexitprogram(exitprogram_),
	master(master_),
	m_bankUI(NULL)

{
	qDebug() << "Searching for controls...";
	ControlIterator it;
	for (it = ControlContainer::getRoot()->getControls().begin();
			it != ControlContainer::getRoot()->getControls().end();
			it++) {
		qDebug() << "Found control: " << QString::fromStdString((*it)->getAbsoluteId());
	}

	qDebug() << "Trying to find control Master.Volume";
	qDebug() << "Returned " << ControlContainer::getRoot()->findControl("Master.Volume");
	
	setupUi(this);


}


void MasterUI::on_action_Quit_triggered()
{
	*Pexitprogram = 1;
}
#include "masterui.moc"
