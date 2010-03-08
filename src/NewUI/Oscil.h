/*
  ZynAddSubFX - a software synthesizer

  Oscil.h - The oscillator gui

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

#ifndef OSCIL_H
#define OSCIL_H

#include "ui_Oscil.h"
#include <QDialog>

class Oscil : public QDialog, public Ui::OscilUi
{
    Q_OBJECT

    public:
        Oscil(QWidget *parent=NULL);

};

#endif // OSCIL_H
