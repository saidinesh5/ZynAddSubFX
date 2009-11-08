#ifndef _LFOBOX_H_
#define _LFOBOX_H_

#include "UnitBox.h"
#include "ui_LFOBox.h"
#include "../Misc/Master.h"

class LFOBox:public UnitBox, Ui::LFOBox
{
    Q_OBJECT

    public:
        LFOBox(QWidget *parent = NULL);
        void setSources(LFOParams *params);
};

#endif /* ifndef _LFOBOX_H_ */

