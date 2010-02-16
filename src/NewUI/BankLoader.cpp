#include "BankLoader.h"
#include "ControlHelper.cpp"

BankLoader::BankLoader(QString partId, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    ControlHelper *instrumentHelper = new ControlHelper(instrumentList);

    connect(instrumentHelper, SIGNAL(optionsChanged(QStringList)),
            this, SLOT(slotBankChanged(QStringList)));

    connect(instrumentList, SIGNAL(currentRowChanged(int)),
            instrumentHelper, SLOT(setValue(int)));


    setProperty("absoluteControlId", partId);
    setWindowTitle(partId.split(".").last());
}

void BankLoader::slotBankChanged(QStringList instruments)
{
    instrumentList->clear();
    instrumentList->addItems(instruments);
}

#include "BankLoader.moc"
// vim: sw=4 sts=4 et tw=100
