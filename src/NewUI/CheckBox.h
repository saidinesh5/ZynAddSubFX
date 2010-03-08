/*
  ZynAddSubFX - a software synthesizer

  CheckBox.h - A checkbox

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

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>

class CheckBox:public QCheckBox
{
    Q_OBJECT

    public:
        CheckBox(QWidget *parent = NULL);

    private slots:
        void setChecked(int val);
};

#endif // CHECKBOX_H

