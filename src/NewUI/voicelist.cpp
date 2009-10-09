#include "voicelist.h"
#include "collapsableframe.h"
#include "voicewidget.h"
#include <QtDebug>

VoiceList::VoiceList(QString absoluteId, QWidget *parent)
    :QDialog(parent)
{
    setProperty("absoluteControlId", absoluteId);

    setupUi(this);

    QWidget *mainWidget = new QWidget;
    scrollArea->setWidget(mainWidget);

    QLayout *lay = new QVBoxLayout(mainWidget);

    Node *node   = Node::find(absoluteId.toStdString());
    if(node)
        for(int i = 0; i < node->getChildren().size(); ++i) {
            VoiceWidget      *w = new VoiceWidget();
            CollapsableFrame *f = new CollapsableFrame(this, w);
            f->title->setText("<h3>"
                              + QString::fromStdString(node->getChildren()[i]->
                                                       getId()) + "</h3>");
            lay->addWidget(f);
            w->setProperty("controlId",
                           QString::fromStdString(node->getChildren()[i]->getId()));
        }

}

// vim: sw=4 sts=4 et tw=100

