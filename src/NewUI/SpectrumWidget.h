/*
  ZynAddSubFX - a software synthesizer

  SpectrumWidget.h - A custom widget that draws an osciallator spectrum

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

#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QWidget>
#include "ArrayControlHelper.h"
#include "../Controls/ArrayControl.h"

class SpectrumWidget : public QWidget
{
    Q_OBJECT

    public:
        SpectrumWidget(QWidget *parent = NULL);
        void paintEvent(class QPaintEvent* event);

    private slots:
        void readArray(ArrayControl* array);

    private:
        REALTYPE *m_data;
        int m_size;

};

#endif // SPECTRUMWIDGET_H
