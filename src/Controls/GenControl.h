/*
  ZynAddSubFX - a software synthesizer

  GenControl.h - Control base class
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
#ifndef _GEN_CONTROL_H
#define _GEN_CONTROL_H

#include <string>
#include "Node.h"

/**A control for a parameter within the program*/
class GenControl : public Node
{
public:
    enum controlType{
        Real, Char, Int, Bool
    };
    virtual void setValue(char val)=0;
    virtual std::string getString() const=0;
    enum controlType getType(){return type;};

    virtual void requestValue()=0;
protected:
    GenControl(Node *parent, std::string id,enum controlType ntype)
        :Node(parent,id),type(ntype)
    {};
    virtual ~GenControl() {}

private:
    enum controlType type;

    //std::string m_description;
};

#endif

