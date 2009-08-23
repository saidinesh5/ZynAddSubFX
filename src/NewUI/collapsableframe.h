#ifndef COLLAPSABLEFRAME_H
#define COLLAPSABLEFRAME_H

#include <QFrame>
#include "ui_collapsableframe.h"

class CollapsableFrame : public QFrame, public Ui::CollapsableFrame
{
    Q_OBJECT

    public:
        CollapsableFrame(QWidget *parent, QWidget *expanded);
        void setExpandedWidget(QWidget *widget);
        void setCollapsedWidget(QWidget *widget);

    public slots:
        void toggleCollapsed();
        void deleteMe();
        void on_enabledCheck_stateChanged(int state);

    private:
        QWidget *expanded;
        QList<QWidget*> showOnExpand;
        QList<QWidget*> hideOnExpand;

};

#endif // COLLAPSABLEFRAME_H
