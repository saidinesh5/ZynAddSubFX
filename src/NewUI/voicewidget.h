#ifndef VOICEWIDGET_H
#define VOICEWIDGET_H

#include <QWidget>
#include "ui_voicewidgetexpanded.h"
#include "ui_voicewidgetcollapsed.h"

class VoiceWidget : public QWidget, public Ui::Voicewidget
{
    public:
        VoiceWidget(QWidget *parent = NULL);
};

class CollapsedVoiceWidget : public QWidget, public Ui::CollapsedVoiceWidget
{

    public:
        CollapsedVoiceWidget(QWidget *parent = NULL);
};

#endif // VOICEWIDGET_H
