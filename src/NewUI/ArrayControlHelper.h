#ifndef ARRAYCONTROLHELPER_H
#define ARRAYCONTROLHELPER_H

#include "ControlHelper.h"
#include "../Controls/ArrayControl.h"

class ArrayControlHelper : public ControlHelper
{
    Q_OBJECT

    public:
        ArrayControlHelper(QObject *parent);
        virtual void connectedEvent();
        virtual void disconnectedEvent();
        virtual void newValueEvent();

    signals:
        void arrayUpdated(ArrayControl *control);

};

#endif // ARRAYCONTROLHELPER_H
