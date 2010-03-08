/*
  ZynAddSubFX - a software synthesizer

  ComboBox.cpp - A combobox widget

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "ComboBox.h"
#include "ControlHelper.h"
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
        //if (pixmapKeys.contains(item)) {
        if (false) {
            addItem(QPixmap(pixmapKeys.value(item)), item);
        } else {
            addItem(item);
        }
    }
}
#include "ComboBox.moc"
// vim: sw=4 sts=4 et tw=100

