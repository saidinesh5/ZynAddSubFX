#include "GroupBox.h"
#include "ControlHelper.h"
#include <QPushButton>
#include <QtDebug>

GroupBox::GroupBox(QWidget *parent)
    : QGroupBox(parent)
{
    QPushButton *b = new QPushButton("Reset", this);
    b->resize(50, b->height());
    b->setFocusPolicy(Qt::NoFocus);

    connect(b, SIGNAL(clicked()),
            this, SLOT(slotReset()));
}

void GroupBox::slotReset()
{
    QList<ControlHelper *> controlHelpers =
        findChildren<ControlHelper *>();

    foreach(ControlHelper * helper, controlHelpers)
    {
        helper->defaults();
    }
}


#include "GroupBox.moc"
// vim: sw=4 sts=4 et tw=100
