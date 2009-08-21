#include "collapsableframe.h"

CollapsableFrame::CollapsableFrame(QWidget *parent, QWidget *expanded)
    : QFrame(parent),
    expanded(expanded)
{
    setupUi(this);
    connect(collapseButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsed()));

    layout()->addWidget(expanded);
    QList<QWidget*> childs = expanded->findChildren<QWidget*>();
    foreach (QWidget *child, childs) {
        if (child->property("showOnExpand").toBool()) {
            showOnExpand.append(child);
            child->hide();
        }
        if (child->property("hideOnExpand").toBool()) {
            hideOnExpand.append(child);
            child->show();
        }
    }
}

void CollapsableFrame::toggleCollapsed()
{
    if (!expanded) return;

    if (collapseButton->text() == ">") {
        collapseButton->setText("\\/");

        foreach (QWidget *child, showOnExpand) {
            child->show();
        }
        foreach (QWidget *child, hideOnExpand) {
            child->hide();
        }
    } else {
        collapseButton->setText(">");
        foreach (QWidget *child, showOnExpand) {
            child->hide();
        }
        foreach (QWidget *child, hideOnExpand) {
            child->show();
        }
    }
}

#include "collapsableframe.moc"
// vim: sw=4 sts=4 et tw=100
