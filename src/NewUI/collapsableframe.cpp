#include "collapsableframe.h"

CollapsableFrame::CollapsableFrame(QWidget *parent, QWidget *expanded, QWidget *collapsed)
    : QFrame(parent),
    expanded(expanded),
    collapsed(collapsed)
{
    setupUi(this);
    connect(collapseButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsed()));

    layout()->addWidget(expanded);
    layout()->addWidget(collapsed);

    expanded->hide();
}

void CollapsableFrame::toggleCollapsed()
{
    if (!collapsed || !expanded) return;

    if (collapseButton->text() == ">") {
        collapseButton->setText("\\/");
        collapsed->hide();
        expanded->show();
    } else {
        collapseButton->setText(">");
        collapsed->show();
        expanded->hide();
    }
}

#include "collapsableframe.moc"
// vim: sw=4 sts=4 et tw=100
