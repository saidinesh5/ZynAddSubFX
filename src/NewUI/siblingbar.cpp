#include "siblingbar.h"
#include <QEvent>
#include "../Controls/ControlContainer.h"
#include "controlhelper.h"
#include <QSet>
#include <QtDebug>

SiblingBar::SiblingBar(QWidget *parent)
	: QTabBar(parent)
{

}

void SiblingBar::addControlWidgets(QWidget *widget)
{
	QSet<ControlHelper*> newWidgets = widget->findChildren<ControlHelper*>().toSet();


	//add all children controls that arent already in the set/list
	foundControls.unite(newWidgets);
	
	//qDebug() << "We now have total controlwidgets " << foundControls;
	for (QSet<ControlHelper*>::const_iterator it = foundControls.constBegin();
			it != foundControls.constEnd(); ++it) {
		qDebug() << "Control " << *it << " has id " << (*it)->getControlId();
	}
}

void SiblingBar::setChildrenContainer(ControlContainer* container)
{
	if (!container) {
		qDebug() << "NULL-container, returning";
		return;
	}

	ContainerIterator it = container->getContainers().begin();
	while (it != container->getContainers().end()) {
		addTab(QString::fromStdString((*it)->getId()));
		it++;
	}
}

#include "siblingbar.moc"
