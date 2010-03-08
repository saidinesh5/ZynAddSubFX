/*
  ZynAddSubFX - a software synthesizer

  PropertyWatch.h - A class that will watch all qt events, updating helpers if the
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

#ifndef PROPERTYWATCH_H
#define PROPERTYWATCH_H

#include <QObject>

class PropertyWatch : public QObject
{
    public:
        PropertyWatch(QObject *parent);

        /**
         * @brief The Qt event handler, reimplementedfor watching the parent widget for property
         * changes that would change the current control etc.
         */
        bool eventFilter(QObject *watched, QEvent *event);
};

#endif // PROPERTYWATCH_H
