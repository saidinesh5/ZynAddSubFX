#ifndef COLLAPSABLEFRAME_H
#define COLLAPSABLEFRAME_H

#include <QFrame>
#include "ui_collapsableframe.h"

class CollapsableFrame : public QFrame, public Ui::CollapsableFrame
{
    Q_OBJECT

    public:
        CollapsableFrame(QWidget *parent = NULL);

    public slots:
        void toggleCollapsed();
};

#endif // COLLAPSABLEFRAME_H
