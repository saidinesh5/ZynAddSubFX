/*
  ZynAddSubFX - a software synthesizer

  VoiceParameters.h - Dialog for once voice's parameters

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
