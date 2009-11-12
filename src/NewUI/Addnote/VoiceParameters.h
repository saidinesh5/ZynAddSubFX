#ifndef VOICEPARAMETERS_H
#define VOICEPARAMETERS_H

#include <QDialog>
#include "ui_VoiceParameters.h"

class VoiceParameters : public QDialog,public Ui::VoiceParameters
{
    public:
        VoiceParameters(QWidget *parent = NULL);
};

#endif // VOICEPARAMETERS_H
