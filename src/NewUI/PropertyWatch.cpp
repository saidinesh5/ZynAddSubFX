/*
  ZynAddSubFX - a software synthesizer

  PropertyWatch.cpp - A class that will watch all qt events, updating helpers if the
  controlId/absoluteControlId properties are changed.

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

#include "PropertyWatch.h"
#include <QCoreApplication>
#include "ControlHelper.h"

PropertyWatch::PropertyWatch(QObject *parent)
    :QObject(parent)
{
    QCoreApplication::instance()->installEventFilter(this);
}

bool PropertyWatch::eventFilter(QObject *object, QEvent *event)
{
    //remember that this functions is called for _all_ qt events, be efficient!

    if(event->type() == QEvent::QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *ev =
            static_cast<QDynamicPropertyChangeEvent *>(event);
        if((ev->propertyName() == "absoluteControlId")
           || (ev->propertyName() == "controlId")) {
            QList<ControlHelper *> controlHelpers =
                object->findChildren<ControlHelper *>();
            foreach(ControlHelper * helper, controlHelpers)
            {
                helper->updateControlId();
            }
        }
    }
    return false;
}

// vim: sw=4 sts=4 et tw=100
