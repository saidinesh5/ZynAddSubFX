/*
  ZynAddSubFX - a software synthesizer

  ArrayControlHelper.h - A ControlHelper variant for arrays

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

#ifndef ARRAYCONTROLHELPER_H
#define ARRAYCONTROLHELPER_H

#include "ControlHelper.h"
#include "../Controls/ArrayControl.h"

class ArrayControlHelper : public ControlHelper
{
    Q_OBJECT

    public:
        ArrayControlHelper(QObject *parent);
        virtual void connectedEvent();
        virtual void disconnectedEvent();
        virtual void newValueEvent();

    signals:
        void arrayUpdated(ArrayControl *control);

};

#endif // ARRAYCONTROLHELPER_H
