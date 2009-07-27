#include "voicelist.h"
#include "collapsableframe.h"
#include "voicewidget.h"
#include <QtDebug>

VoiceList::VoiceList(QString partPath, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowTitle(partPath);

    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);

    QLayout *lay = new QVBoxLayout(mainWidget);

    //lay->setSizeConstraint(QLayout::SetMinAndMaxSize);

    for (int i = 0; i < 6; ++i) {
        CollapsableFrame *f = new CollapsableFrame(this, new VoiceWidget(), new CollapsedVoiceWidget());

        QSet<ControlHelper*> controlHelpers = f->findChildren<ControlHelper*>().toSet();
        for (QSet<ControlHelper*>::const_iterator it = controlHelpers.constBegin();
            it != controlHelpers.constEnd(); ++it) {

            (*it)->setControl(QString()); //clear out the current control
            if ((*it)->controlId().isEmpty()) continue;

            (*it)->setControl(partPath + "." + (*it)->controlId());
        }

        lay->addWidget(f);
    }

}

// vim: sw=4 sts=4 et tw=100
