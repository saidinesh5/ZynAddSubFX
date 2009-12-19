#ifndef _ENVELOPEBOX_H_
#define _ENVELOPEBOX_H_

#include "UnitBox.h"
#include "ui_EnvelopeBox.h"
#include "../Misc/Master.h"
#include "../Params/EnvelopeParams.h"

class EnvelopeBox:public UnitBox, Ui::EnvelopeBox
{
    Q_OBJECT

    public:
        EnvelopeBox(QWidget *parent = NULL);
        void setSources(EnvelopeParams *params);
};

#endif /* ifndef _ENVELOPEBOX_H_ */

