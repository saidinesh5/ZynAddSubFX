#include "collapsableframe.h"

CollapsableFrame::CollapsableFrame(QWidget *parent)
    : QFrame(parent)
{
    setupUi(this);
    connect(collapseButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsed()));
}

void CollapsableFrame::toggleCollapsed()
{

}

#include "collapsableframe.moc"
// vim: sw=4 sts=4 et tw=100
