#include "siblingbar.h"
#include <QEvent>
#include "../Controls/ControlContainer.h"
#include "controlhelper.h"
#include <QSet>
#include <QtDebug>

SiblingBar::SiblingBar(QWidget *parent)
	: QTabBar(parent),
	m_childContainer(NULL)
{
	connect(this, SIGNAL(currentChanged(int)),
			this, SLOT(updateFoundControls()));
}

void SiblingBar::addControlWidgets(QWidget *widget)
{
	QSet<ControlHelper*> newWidgets = widget->findChildren<ControlHelper*>().toSet();


	//add all children controls that arent already in the set/list
	m_foundControls.unite(newWidgets);
	
	//qDebug() << "We now have total controlwidgets " << m_foundControls;
	for (QSet<ControlHelper*>::const_iterator it = m_foundControls.constBegin();
			it != m_foundControls.constEnd(); ++it) {
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

	m_childContainer = container;
}

void SiblingBar::updateFoundControls()
{
	qDebug() << "updateFoundControls";

	if (!m_childContainer) return;

	QString childAbsoluteId = QString::fromStdString(m_childContainer->getContainers().at(currentIndex())->getAbsoluteId());

	for (QSet<ControlHelper*>::const_iterator it = m_foundControls.constBegin();
			it != m_foundControls.constEnd(); ++it) {

		if ((*it)->relativeControlId().isEmpty()) continue;

		(*it)->setControl(childAbsoluteId + "." + (*it)->relativeControlId());
		//qDebug() << "Control " << *it << " changed to " << (*it)->getControlId();

	}

}

#include "siblingbar.moc"
