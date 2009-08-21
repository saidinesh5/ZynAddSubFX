#ifndef VOICEWIDGET_H
#define VOICEWIDGET_H

#include <QWidget>
#include "ui_voicewidgetexpanded.h"

class VoiceWidget : public QWidget, public Ui::Voicewidget
{
    public:
        VoiceWidget(QWidget *parent = NULL);
};

#endif // VOICEWIDGET_H
