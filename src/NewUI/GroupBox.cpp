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

    QToolButton *copy = new QToolButton(this);
    copy->move(25, 5);
    copy->resize(20, 20);
    copy->setFocusPolicy(Qt::NoFocus);
    copy->setArrowType(Qt::NoArrow);
    copy->setAutoRaise(true);
    copy->setPopupMode(QToolButton::InstantPopup);
    copy->setText("C");

    QToolButton *paste = new QToolButton(this);
    paste->move(45, 5);
    paste->resize(20, 20);
    paste->setFocusPolicy(Qt::NoFocus);
    paste->setArrowType(Qt::NoArrow);
    paste->setAutoRaise(true);
    paste->setPopupMode(QToolButton::InstantPopup);
    paste->setText("P");

    QAction *action = new QAction("Reset to defaults", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotReset()));

    action = new QAction("Copy...", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotCopy()));
    connect(copy, SIGNAL(clicked()),
            action, SLOT(trigger()));

    action = new QAction("Paste...", b);
    b->addAction(action);
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotPaste()));
    connect(paste, SIGNAL(clicked()),
            action, SLOT(trigger()));
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

void GroupBox::slotCopy()
{
    qDebug() << "copy! TODO";
}

void GroupBox::slotPaste()
{
    qDebug() << "paste! TODO";
}

#include "GroupBox.moc"
// vim: sw=4 sts=4 et tw=100
