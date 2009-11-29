#include "SiblingBar.h"
#include <QEvent>
#include "../Controls/Node.h"
#include "ControlHelper.h"
#include <QSet>
#include <QtDebug>
#include <QMouseEvent>
#include <QMenu>

SiblingBar::SiblingBar(QWidget *parent)
    :QTabBar(parent),
      m_childContainer(NULL),
      m_controlsWidget(NULL)
{
    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(updateFoundControls()));
}

void SiblingBar::setControlsWidget(QWidget *widget)
{
    m_controlsWidget = widget;
}

void SiblingBar::setNode(Node *container)
{
    if(!container) {
        qDebug() << "NULL-container, returning";
        return;
    }

    NodeIterator it = container->getChildren().begin();
    while(it != container->getChildren().end()) {
        addTab(QString::fromStdString((*it)->getId()));
        it++;
    }

    m_childContainer = container;

    updateFoundControls();
}

void SiblingBar::updateFoundControls()
{
    qDebug() << "updateFoundControls";

    if(!m_childContainer || !m_controlsWidget)
        return;

    QString childAbsoluteId;

    if(m_childContainer->getChildren().size())
        childAbsoluteId = QString::fromStdString(
            m_childContainer->getChildren().at(currentIndex())->getAbsoluteId());
    else
        childAbsoluteId = QString();
    m_controlsWidget->setProperty("absoluteControlId", childAbsoluteId);
}

void SiblingBar::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() != Qt::RightButton))
        return;
    if(!m_childContainer)
        return;

    int tabIndex = -1;
    for(int i = 0; i < count(); ++i)
        if(tabRect(i).contains(event->pos())) {
            tabIndex = i;
            break;
        }


    QMenu menu(this);

    QList<QAction *> typeActions;
    for(int i = 0; i < m_childContainer->getTypes().size(); ++i)
        typeActions.append(menu.addAction("&Add new "
                                          + QString::fromStdString(
                                              m_childContainer->getTypes().at(i))));

    QAction *duplicate = NULL, *deleteAction = NULL;
    if(tabIndex != -1)
        duplicate = menu.addAction("&Duplicate");
    if(tabIndex != -1)
        deleteAction = menu.addAction("&Delete");

    QAction *response = menu.exec(event->globalPos());

    if(response == duplicate) {}
    else
    if(response == deleteAction) {
        QString tabname = tabText(tabIndex);
        removeTab(tabIndex);
        updateFoundControls();
        m_childContainer->removeChild(tabname.toStdString());
        return;
    }

    int typeIndex = typeActions.indexOf(response);
    if(typeIndex != -1) {
        std::string stdChild = m_childContainer->createChild(typeIndex);
        QString     childId  = QString::fromStdString(stdChild);
        if(!childId.isEmpty()) {
            Node::lock();
            Node *newContainer = Node::get(stdChild);
            addTab(QString::fromStdString(newContainer->getId()));
            Node::unlock();
        }
        qDebug() << "Created child at " << childId;
    }
}

QString SiblingBar::getCurrentChild()
{
    if(!m_childContainer)
        return QString();
    if(!m_childContainer->getChildren().size())
        return QString();

    return QString::fromStdString(m_childContainer->getChildren().at(
                                      currentIndex())->getAbsoluteId());
}

#include "SiblingBar.moc"

