#include "combobox.h"
#include "controlhelper.h"
#include <QtDebug>
#include <QMap>

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

    setIconSize(QSize(64,64));
}

void ComboBox::setOptions(QStringList options)
{

    static QMap<QString,QString> pixmapKeys;
    if (pixmapKeys.empty()) {
        pixmapKeys["Lowpass 1 pole"] = ":/images/lpf1.png";
        pixmapKeys["Highpass 1 pole"] = ":/images/hpf1.png";
        pixmapKeys["Lowpass 2 pole"] = ":/images/lpf2.png";
        pixmapKeys["Highpass 2 pole"] = ":/images/hpf2.png";
    }

    qDebug() << "setOptions";
    clear();
    foreach(const QString &item, options)
    {
        if (pixmapKeys.contains(item)) {
            addItem(QPixmap(pixmapKeys.value(item)), item);
        } else {
            addItem(item);
        }
    }
}
#include "combobox.moc"
// vim: sw=4 sts=4 et tw=100

