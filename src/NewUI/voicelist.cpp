#include "voicelist.h"
#include "collapsableframe.h"
#include "voicewidget.h"
#include <QtDebug>

VoiceList::VoiceList(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);

    QLayout *lay = new QVBoxLayout(mainWidget);

    //lay->setSizeConstraint(QLayout::SetMinAndMaxSize);

    for (int i = 0; i < 6; ++i) {
        CollapsableFrame *f = new CollapsableFrame(this, new VoiceWidget(), new CollapsedVoiceWidget());
        lay->addWidget(f);
    }

}

// vim: sw=4 sts=4 et tw=100