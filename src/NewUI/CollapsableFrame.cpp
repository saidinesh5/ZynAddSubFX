#include "CollapsableFrame.h"
#include "ControlHelper.h"

CollapsableFrame::CollapsableFrame(QWidget *parent, QWidget *expanded)
    :QFrame(parent),
      expanded(expanded)
{
    setupUi(this);
    connect(collapseButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsed()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteMe()));

    layout()->addWidget(expanded);
    QList<QWidget *> childs = expanded->findChildren<QWidget *>();
    foreach(QWidget * child, childs)
    {
        if(child->property("showOnExpand").toBool()) {
            showOnExpand.append(child);
            child->hide();
        }
        if(child->property("hideOnExpand").toBool()) {
            hideOnExpand.append(child);
            child->show();
        }
    }
}

bool CollapsableFrame::isCollapsed()
{
    return (collapseButton->text() == ">");
}

void CollapsableFrame::toggleCollapsed()
{
    if(!expanded)
        return;

    if(isCollapsed()) {
        collapseButton->setText("\\/");

        foreach(QWidget * child, showOnExpand)
        {
            child->show();
        }
        foreach(QWidget * child, hideOnExpand)
        {
            child->hide();
        }
    }
    else {
        collapseButton->setText(">");
        foreach(QWidget * child, showOnExpand)
        {
            child->hide();
        }
        foreach(QWidget * child, hideOnExpand)
        {
            child->show();
        }
    }
}

void CollapsableFrame::deleteMe()
{
    if(!expanded)
        return;

    QString id = ControlHelper::findComponentPath(expanded);
    if(id.isEmpty())
        return;

    Node *node = Node::find(id.toStdString());
    if(!node)
        return;

    node->removeFromParent();
}

void CollapsableFrame::on_enabledCheck_stateChanged(int state)
{
    if(!expanded)
        return;

    if(state == Qt::Checked)
        expanded->setEnabled(true);
    else
    if(state == Qt::Unchecked)
        expanded->setEnabled(false);
}

#include "CollapsableFrame.moc"
// vim: sw=4 sts=4 et tw=100

