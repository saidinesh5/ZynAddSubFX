/*
  ZynAddSubFX - a software synthesizer

  Control.cpp - Control template
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
#include "JobClasses.h"
#include "DescRanger.h"

using namespace std;

DescRanger::DescRanger(Node *parent, std::string id, int defaultval, int min, int max)
    :Control<int>(parent, id, defaultval)
{
    this->min = min;
    this->max = max;
}

DescRanger::~DescRanger()
{

}

void DescRanger::addpar(XMLwrapper *xml, const std::string& name)
{
    xml->addpar(name, getValue());
}

void DescRanger::getpar(XMLwrapper *xml, const std::string& name)
{
    setValue(xml->getpar(name, getValue(), getMin(), getMax()));
}

