#include "VoiceWidget.h"
#include "VoiceParameters.h"
#include "ControlHelper.h"
#include <QtDebug>

VoiceWidget::VoiceWidget(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
}

void VoiceWidget::on_showParameters_clicked()
{
    VoiceParameters *params = new VoiceParameters();
    params->setProperty("absoluteControlId", ControlHelper::findComponentPath(parent()));
    params->show();
}

#include "VoiceWidget.moc"
// vim: sw=4 sts=4 et tw=100

