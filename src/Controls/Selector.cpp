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
#include "Selector.h"

using namespace std;

//type,location,mutex
Selector::Selector(Node *parent, std::string id, int defaultval)
    :Control<int>(parent,id,defaultval)
{}

Selector::~Selector()
{

}

string Selector::getString()const
{
    return getOption(value);
}

string Selector::getOption(int idx) const
{
    string tmp;
    pthread_mutex_lock(&localMute);
    tmp = options[idx];
    pthread_mutex_unlock(&localMute);
    return tmp;
}

int Selector::numOptions() const
{
    int tmp;
    pthread_mutex_lock(&localMute);
    tmp = options.size();
    pthread_mutex_unlock(&localMute);
    return tmp;
}

void Selector::addOption(string option)
{
    pthread_mutex_lock(&localMute);
    options.push_back(option);
    pthread_mutex_unlock(&localMute);
}

void Selector::clearOptions()
{
    pthread_mutex_lock(&localMute);
    options.clear();
    pthread_mutex_unlock(&localMute);
}

