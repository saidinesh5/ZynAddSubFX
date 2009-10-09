#include "combobox.h"
#include "controlhelper.h"
#include <QtDebug>

ComboBox::ComboBox(QWidget *parent)
    :QComboBox(parent)
{
    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(activated(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setCurrentIndex(int)));
    connect(helper, SIGNAL(optionsChanged(QStringList)),
            this, SLOT(setOptions(QStringList)));
}

void ComboBox::setOptions(QStringList options)
{
    qDebug() << "setOptions";
    clear();
    foreach(const QString &item, options)
    {
        addItem(item);
    }
}
#include "combobox.moc"
// vim: sw=4 sts=4 et tw=100

