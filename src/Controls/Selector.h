/*
  ZynAddSubFX - a software synthesizer

  Ranger.h - Control base class
  Copyright (C) 2009-2009 Mark McCurry
  Author: Mark McCurry

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
#ifndef SELECTOR_H
#define SELECTOR_H

#include <string>
#include <vector>
#include "Control.h"

typedef std::vector<std::string> StringVector;

/**
 * A Selection Representation
 * that is safe in a threaded environment
 */
class Selector:public Control<int>
{
    public:
        Selector(Node *parent, std::string id, int defaultval);
        ~Selector();

        std::string getString() const;

        std::string getOption(int idx) const;
        int numOptions() const;
        void addOption(std::string option);
        void clearOptions();

        inline void operator=(int v) {
            setValue(v);
        }

    private:
        StringVector options;
};

#endif

