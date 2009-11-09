#include "GroupBox.h"
#include "ControlHelper.h"
#include <QtDebug>
#include <QToolButton>
#include <QAction>

GroupBox::GroupBox(QWidget *parent)
    : QGroupBox(parent)
{


    QToolButton *b = new QToolButton(this);
    b->move(5, 5);
    b->resize(20, 20);
    b->setFocusPolicy(Qt::NoFocus);
    b->setArrowType(Qt::NoArrow);
    b->setAutoRaise(true);
    b->setPopupMode(QToolButton::InstantPopup);

    QAction *action = new QAction("Reset to defaults", b);
    b->addAction(action);

    connect(action, SIGNAL(triggered()),
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
