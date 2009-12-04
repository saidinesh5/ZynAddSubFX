#include "VoiceList.h"
#include "CollapsableFrame.h"
#include "VoiceWidget.h"
#include <QtDebug>

VoiceList::VoiceList(QString absoluteId, QWidget *parent)
    :QDialog(parent)
{
    setProperty("absoluteControlId", absoluteId);

    setupUi(this);

    Node *node   = Node::get(absoluteId.toStdString());

    partBar->setControlsWidget(voiceWidget);
    partBar->setNode(node);
}

// vim: sw=4 sts=4 et tw=100

