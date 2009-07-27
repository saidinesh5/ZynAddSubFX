#include "siblingbar.h"
#include <QEvent>
#include "../Controls/Node.h"
#include "controlhelper.h"
#include <QSet>
#include <QtDebug>
#include <QMouseEvent>
#include <QMenu>

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

void SiblingBar::setNode(Node* container)
{
    if (!container) {
        qDebug() << "NULL-container, returning";
        return;
    }

    NodeIterator it = container->getChildren().begin();
    while (it != container->getChildren().end()) {
        addTab(QString::fromStdString((*it)->getId()));
        it++;
    }

    m_childContainer = container;

    updateFoundControls();

}

void SiblingBar::updateFoundControls()
{
    qDebug() << "updateFoundControls";

    if (!m_childContainer) return;

    QString childAbsoluteId;

    if (m_childContainer->getChildren().size()) {
        childAbsoluteId = QString::fromStdString(m_childContainer->getChildren().at(currentIndex())->getAbsoluteId());
    } else {
        childAbsoluteId = QString();
    }

    for (QSet<ControlHelper*>::const_iterator it = m_foundControls.constBegin();
            it != m_foundControls.constEnd(); ++it) {

        (*it)->setControl(QString()); //clear out the current control

        if (childAbsoluteId.isEmpty()) {
            qDebug() << "Cleared out control";
            continue;
        }

        if ((*it)->controlId().isEmpty()) continue;

        (*it)->setControl(childAbsoluteId + "." + (*it)->controlId());
        qDebug() << "Control " << *it << " changed to " << (*it)->getControlId();

    }

}

void SiblingBar::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() != Qt::RightButton)) return;
    if (!m_childContainer) return;

    int tabIndex = -1;
    for (int i = 0; i < count(); ++i) {
        if (tabRect(i).contains(event->pos())) {
            tabIndex = i;
            break;
        }

    }

    QMenu menu(this);

    QList<QAction*> typeActions;
    for (int i = 0; i < m_childContainer->getTypes().size(); ++i) {
        typeActions.append(menu.addAction("&Add new " + QString::fromStdString(m_childContainer->getTypes().at(i))));
    }

    QAction *duplicate = NULL, *deleteAction = NULL;
    if (tabIndex != -1) duplicate = menu.addAction("&Duplicate");
    if (tabIndex != -1) deleteAction = menu.addAction("&Delete");

    QAction *response = menu.exec(event->globalPos());

    if (response == duplicate) {

    } else if (response == deleteAction) {
        QString tabname = tabText(tabIndex);
        removeTab(tabIndex);
        updateFoundControls();
        m_childContainer->removeChild(tabname.toStdString());
        return;
    }

    int typeIndex = typeActions.indexOf(response);
    if (typeIndex != -1) {
        std::string stdChild = m_childContainer->createChild(typeIndex);
        QString childId = QString::fromStdString(stdChild);
        if (!childId.isEmpty()) {
            Node *newContainer = Node::find(stdChild);
            addTab(QString::fromStdString(newContainer->getId()));
        }
        qDebug() << "Created child at " << childId;
    }

}

QString SiblingBar::getCurrentChild()
{
    if (!m_childContainer) return QString();
    if (!m_childContainer->getChildren().size()) return QString();

    return QString::fromStdString(m_childContainer->getChildren().at(currentIndex())->getAbsoluteId());
}

#include "siblingbar.moc"
