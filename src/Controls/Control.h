/*
  ZynAddSubFX - a software synthesizer

  Control.h - Control base class
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
#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <vector>
#include "ControlContainer.h"

/**A control for a parameter within the program*/
class Control
{
public:
    Control(class ControlContainer *parent, std::string id, std::string name, char ndefaultval);/**\todo create proper initialization list*/
    ~Control() {};
    /**Return the string, which represents the internal value
     * @return a string representation of the current value*/
    virtual std::string getString()const=0;
    /**Set the Control to the given value
     * @param nval A number 0-127*/
    virtual void setValue(char nval)=0;
    /**Return the midi value (aka the char)
     * @return the current value*/
    virtual char getValue()const=0;

    const std::string& getId();
    const std::string getAbsoluteId();

    void registerUser(class ControlUser *user);

    /** Will lock the Control until it is ulocked
     *
     * Locking a Control will Stop it from having
     * its internal data altered*/
    void lock();
    /** Will unlock the Control
     *
     * This will also update the Control
     * if something attempted to update it while it was locked*/
    void ulock();
private:
    char defaultval;/**<Default value of the Control*/
    char lockqueue; /**<The value is that is stored, while the Control is locked
                         * and something attempts to update it*/
    bool locked;//upgrade this to a integer lock level if problems occur
    std::vector<ControlUser*> m_users;
    std::string m_id, m_description;
    class ControlContainer *m_parent;
};

#endif

