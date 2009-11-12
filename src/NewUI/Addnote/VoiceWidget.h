#ifndef VOICEWIDGET_H
#define VOICEWIDGET_H

#include <QWidget>
#include "ui_VoiceWidgetExpanded.h"

class VoiceWidget:public QWidget, public Ui::Voicewidget
{
    Q_OBJECT

    public:
        VoiceWidget(QWidget *parent = NULL);

    private slots:
        void on_showParameters_clicked();

};

#endif // VOICEWIDGET_H

