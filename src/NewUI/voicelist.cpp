#include "voicelist.h"
#include "collapsableframe.h"

VoiceList::VoiceList(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QLayout *lay = new QVBoxLayout(scrollArea);
    scrollArea->setLayout(lay);
    lay->addWidget(new CollapsableFrame());
    lay->addWidget(new CollapsableFrame());
    lay->addWidget(new CollapsableFrame());
}

// vim: sw=4 sts=4 et tw=100
